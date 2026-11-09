#include "base/base_include.h"
#include "lexer.h"
#include "compiler.h"
#include "messages.h"
#include "parse_table.h"
#include "parser.h"
#include "lexer.cpp"
#include "parser.cpp"
#include "base/base_raddbg.h"

template <typename T>
internal T *peek_forward_rec(T *node, s32 steps) {
	if(steps > 0) return peek_forward_rec(node->next, steps - 1);
	return node;
}

internal b32 is_nil_module(Module *module) {return (module == NULL || module == &nil_module);}

internal b32 detect_import_cycle_in_modules(ModuleList *modules) {
	for(Module *fast = modules->current; !is_nil_module(fast) && !is_nil_module(fast->next) ; fast = peek_forward_rec(fast, 1))
	{
		for(Module *slow = modules->current; !is_nil_module(slow) && is_nil_module(slow->next->next); slow = peek_forward_rec(slow, 2)) {
			return false;
		}
	}
	return false;
}

internal String8 consume_param(String8 argument)
{
	u64 equal_position              = 0;
	b32 found_equal_sign            = Str8FindNeedle(&argument, 0, "=", &equal_position);
	if(found_equal_sign) argument      = str8_chop_left(argument, equal_position + 1);
	else {
		Log("wrong compiler param");
		return str8_zero();
	}
	return argument;
}


// NOTE: we return a list a of  all pound sign values  that we found  within the current bufferr
// when find any 1..n we return the next tokens in a String8List.
internal String8List *handle_directives(Arena *arena, String8 buffer, String8 directive_marker, String8 directive_keyword)
{
	Assert(buffer.size);
	String8List *directive_values = PushStructZero(arena, String8List);
	
	b32 found_marker = false;
	u64 marker_offset = 0;
	found_marker = str8_find(&buffer, 0, directive_marker, &marker_offset);
	
	while(found_marker)
	{
		// TODO: think about how we delete the keywords because they should get handled here
		buffer = str8_chop_left(buffer, marker_offset + 1);
		
		String8 directive_word = {};
		u64 import_offset = 0;
		
		//- get word with single delimiter
		{
			String8 word_delimiter = str8(" ");
			u64 word_length = 0;
			b32 reached_delimiter = false;
			
			ScratchArena scratch = scratch_start(arena);
			
			String8 word_cursor = PushString8(scratch.arena, buffer.size);
			str8_copy(&word_cursor, &buffer);
			for(u32 char_index = 0; (!reached_delimiter) && (char_index < word_cursor.size); ++char_index, word_length = char_index) {
				word_cursor.data += 1;
				reached_delimiter = str8_is_any_match(word_cursor, word_delimiter);
			}
			
			scratch_end(&scratch);
			
			Assert(word_length != 0);
			String8 word = PushString8(arena, word_length);
			word = str8_create_substr(&buffer, 0, word_length);
			directive_word = word;
			
			import_offset += word_length;
		}
		
		
		
		if(str8_is_match(directive_word, directive_keyword)) {
			String8 directive_value = {};
			
			//- get value with multiple delimiters
			{
				u64 value_length = 0;
				b32 reached_delimiter = false;
				ScratchArena scratch = scratch_start(arena);
				String8List *value_delimiters = PushStructZero(scratch.arena, String8List);
				
				// push regular delimiters, maybe i should make this a str8 function that checks for all the defualt eol delimters, or word seperators? maybe including parans etc. and call that the get word function. it goes until the first delimiter. but the issue with that function is that its too generic
				
				
				{
					
					String8Node *space_node = PushStruct(scratch.arena, String8Node); *space_node = str8_node(" ");
					String8Node *newline_node    = PushStruct(scratch.arena, String8Node); *newline_node    = str8_node("\n");
					str8_list_push_node(value_delimiters, space_node);
					str8_list_push_node(value_delimiters, newline_node);
				}
				
				String8 value_cursor = PushString8(scratch.arena, buffer.size);
				str8_copy(&value_cursor, &buffer);
				
				for(u32 char_index = 0; (!reached_delimiter) && (char_index < value_cursor.size); ++char_index, value_length = char_index) {
					value_cursor.data += 1;
					FOR_STR8_NODE(value_delimiters, delimiter_node)
					{
						reached_delimiter = str8_is_any_match(value_cursor, delimiter_node->value);
						if(reached_delimiter) {
							break;
						}
					}
				}
				
				
				scratch_end(&scratch);
				
				Assert(value_length != 0);
				String8 substring = PushString8(arena, value_length);
				substring = str8_create_substr(&buffer, import_offset + 1, value_length); // TODO:  0 is wrong. 0 should be the offset from the previous keyword
				directive_value = substring;
				
			}
			
			while (cstring_is_whitespace(*buffer.data)) { buffer = str8_chop_left(buffer, 1); }
			String8Node *directive_value_node = PushStruct(arena, String8Node);
			*directive_value_node = ToString8Node(directive_value);
			str8_list_push_node(directive_values, directive_value_node);
		} else if(str8_is_empty(&directive_word)) {
			Log("Error:" Str8Fmt, Str8Arg(directive_word));
			PANIC("Pound sign was found but no directive defined.");
		}
		
		found_marker = str8_find(&buffer, 0, directive_marker, &marker_offset);
	}
	
	return directive_values;
}

internal void modules_push_new(ModuleList *modules, Module *module) {
	module->next = &nil_module;
	module->prev = &nil_module;
	
	if(is_nil_module(modules->root)) {
		modules->root = module;
	} else {
		module->prev           = modules->current;
		modules->current->next = module;
	}
	
	modules->current = module;
	modules->count++;
}

ENTRYPOINT(argc, argv)
{
    Arena *arena = ArenaCreate(MiB(256));
    AssertLog(arena, "Failed to allocate memory");
	
    String8List *include_directories  = PushStructZero(arena, String8List);
    String8      source_path     = PushString8(arena, 0);
    ModuleList  *modules              = PushStruct(arena, ModuleList);
    ModuleList  *module_list          = PushStruct(arena, ModuleList);
	
    if(argc <= 1) {
		Log("No parameters passed. The compiler is going to find the entry point and take that as the target");
        Log("help [--source=] \"example.jr\" [--include=] [--output=] example[.exe]]\n");
    } else {
        // NOTE: we are bigger then one arguments so we can start assuming that a source file was passed
        for(u64 argument_index = 1; argument_index < (u64)argc; ++argument_index) {
			String8 argument = str8(argv[argument_index]);
            if(str8_starts_with(argument, str8("--include=")) || str8_starts_with(argument, str8("-I="))) {
                String8 include_path = consume_param(argument);
                if(!str8_is_match(include_path, str8_zero())) {
                    String8Node include_node = ToString8Node(include_path);
                    str8_list_push_node(include_directories, &include_node);
                }
            } else if(str8_starts_with(argument, str8("--source="))) {
				source_path = consume_param(argument);
            }
        }
    }
	
	
    // the directory of the input file: everything before the last slash, "." if there is none
    String8 source_directory = {};
	for(u64 char_index = source_path.size; char_index > 0; --char_index) {
        if(cstring_is_slash(source_path.data[char_index - 1])) {
            u64 slash_index = char_index - 1;
			// NOTE: this check makes sure we remove the last trailing slash in case we want to trim by 0
            source_directory  = str8_chop_right(source_path, source_path.size - (slash_index ? slash_index : 1));
            break;
        }
    }
	
    Log("context directory: " Str8Fmt, Str8Arg(source_directory));
	
	
    String8List *directory_files = PushStructZero(arena, String8List);
	Module *target_module = PushStruct(arena, Module);
	target_module->name   = PushString8(arena, source_path.size);
    if(source_path.size != 0)
    {
		//  TODO: doing a +1 to handle the trailing slash. this needs to be fixed from the root
		String8 module_name = str8_chop_left(source_path, (source_directory.size + 1));
		str8_copy(&target_module->name, &module_name);
		// NOTE: we have no owner because we are target
		target_module->owner = &nil_module;
    }
    else
    {
		// TODO: find the files recursivly  first check  if the file is a folder
		// if the file is a folder open the folder and read everything in there
		
		// NOTE: no file should have an owner i think
        directory_files = DirLoadFileNames(arena, &source_directory); // TODO: this should be replaced with a recursive function
        FOR_STR8_NODE(directory_files, file_node) {
            String8 extension = str8_get_file_extension(file_node->value);
            if(str8_is_match(str8(".jr"), extension)) {
                target_module->name = file_node->value;
				
				modules_push_new(modules, target_module);
			}
			
		}
	}
	
	
	// TODO: threading
	{
		
		Module *root_module = PushStructZero(arena, Module);
		root_module->relative_path = source_path;
		root_module->buffer = FileLoad(arena, source_path);
		AssertLog(root_module->buffer.size, "Failed to load source file: " Str8Fmt, Str8Arg(source_path));
		modules_push_new(modules, root_module);
		
		String8 directive_marker = str8("#");
		String8 directive_keyword   = str8("import");
		
		// handle comments
		{
			modules->current->buffer = str8_cut_match(arena,  root_module->buffer,  str8("//"),  str8("\n"));
			modules->current->buffer = str8_cut_match(arena,  root_module->buffer, str8("/*"),  str8("*/"));
		}
		
		String8List *import_paths =  handle_directives(arena, modules->current->buffer,  directive_marker, directive_keyword);
		
#if DEBUG
		AssertLog(import_paths->count, "No imports found, not possible, when testing");
#endif
		
		FOR_STR8_NODE(import_paths, import_node)
		{
			Module *import_module        = PushStruct(arena, Module);
			
			import_module->relative_path = PushString8(arena, import_node->value.size);
			import_module->relative_path.size = 0;
			
			b32 append_ok = str8_append(&import_module->relative_path, import_node->value);
			Assert(append_ok);
			
			import_module->relative_path = str8_insert(arena, import_module->relative_path, str8("./"), 0);
			
			b32 import_exists = FileExists(arena, import_module->relative_path);
			AssertLog(import_exists, "failed to import: " Str8Fmt, Str8Arg(import_module->relative_path));
			
			import_module->name = import_node->value;
			modules_push_new(modules, import_module);
		}
		
		FOR_NODE(module, modules, &nil_module)
		{
			if(module->buffer.size == 0){
                module->buffer = FileLoad(arena, module->relative_path);
                AssertLog(module->buffer.size, "Failed to load module: " Str8Fmt, Str8Arg(module->relative_path));
			}
			
			// TODO: potential metaprogramming
			{ }
			
			
			{
				module->buffer = str8_cut_match(arena,  root_module->buffer,  str8("//"),  str8("\n"));
				module->buffer = str8_cut_match(arena,  root_module->buffer,  str8 ("/*"),  str8("*/"));
			}
			
			
			module->tree =  tokenize_tree(arena, module->buffer);
			AssertLog(module->tree, "Failed to create Token Tree");
		}
	}
	
	return 0;
	
}