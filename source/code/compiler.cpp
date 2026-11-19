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

internal b32 is_nil_module(Module *module) {
    return (module == NULL || module == &nil_module);
}

// NOTE: this was some sort of compiler algorithm for finding import cycles
// but i forgot which one...
internal b32 detect_import_cycle_in_modules(ModuleList *modules) {
    for(Module *fast = modules->current; !is_nil_module(fast) && !is_nil_module(fast->next) ; fast = peek_forward_rec(fast, 1))
    {
		for(Module *slow = modules->current; !is_nil_module(slow) && is_nil_module(slow->next->next); slow = peek_forward_rec(slow, 2))
		{
			return false;
		}
    }
    return false;
}

internal String8 consume_param(Arena *arena, String8 argument)
{
    String8 result = PushString8(arena, argument.size);
    u64 equal_position = 0;
    b32 found_equal_sign = Str8FindNeedle(&argument, 0, "=", &equal_position);
    if(found_equal_sign) result = str8_chop_left(argument, equal_position + 1);
    else {
		Log("wrong compiler param");
		return str8_zero();
    }
    return result;
}

internal String8 consume_param_as_potential_list_using_delimiter(Arena *arena, String8 argument, String8 delimiter)
{
	
    String8 result = PushString8(arena, argument.size);
    u64 equal_position = 0;
    b32 found_equal_sign = Str8FindNeedle(&argument, 0, "=", &equal_position);
    if(found_equal_sign)
    {
		result = str8_chop_left(argument, equal_position + 1);
    }
    else
    {
		Log("wrong compiler param");
		return str8_zero();
    }
    return result;
}

internal b32 proc_table_contains_entrypoint(TokenList *list)
{
	
    FOR_TOKEN_NODE(node, list, &nil_token_node)
    {
		if(str8_is_match(node->token->value, str8("main")))
		{
			return true;
		}
    }
    return false;
	
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
		
		
		
		if(str8_is_match(directive_word, directive_keyword))
		{
			String8 directive_value = {};
			
			//- get value with multiple delimiters
			{
				u64 value_length = 0;
				b32 reached_delimiter = false;
				ScratchArena scratch = scratch_start(arena);
				String8List *value_delimiters = PushStructZero(scratch.arena, String8List);
				
				// push regular delimiters, maybe i should make this a str8 function that checks for all the defualt eol delimters, or word seperator?
				// maybe including parans etc. and call that the get word function. it goes until the first delimiter. but the issue with that function is that its too generic
				{
					String8Node *space_node = PushStruct(scratch.arena, String8Node); *space_node = str8_node(" ");
					String8Node *newline_node    = PushStruct(scratch.arena, String8Node); *newline_node    = str8_node("\n");
					str8_list_push_node(value_delimiters, space_node);
					str8_list_push_node(value_delimiters, newline_node);
				}
				
				
				String8 value_cursor = PushString8(scratch.arena, buffer.size);
				str8_copy(&value_cursor, &buffer);
				
				for(u32 char_index = 0; (!reached_delimiter) && (char_index < value_cursor.size); ++char_index, value_length = char_index)
				{
					value_cursor.data += 1;
					FOR_STR8_NODE(value_delimiters, delimiter_node)
					{
						reached_delimiter = str8_is_any_match(value_cursor, delimiter_node->value);
						if(reached_delimiter)
						{
							break;
						}
					}
				}
				
				
				scratch_end(&scratch);
				
				Assert(value_length != 0);
				String8 substring = PushString8(arena, value_length);
				// TODO:  0 is wrong. 0 should be the offset from the previous keyword
				substring = str8_create_substr(&buffer, import_offset + 1, value_length);
				directive_value = substring;
				
			}
			
			while(cstring_is_whitespace(*buffer.data)) { buffer = str8_chop_left(buffer, 1); }
			String8Node *directive_value_node = PushStruct(arena, String8Node);
			*directive_value_node = ToString8Node(directive_value);
			str8_list_push_node(directive_values, directive_value_node);
		}
		else if(str8_is_empty(&directive_word))
		{
			Log("Error:" Str8Fmt, Str8Arg(directive_word));
			PANIC("Pound sign was found but no directive defined.");
		}
		found_marker = str8_find(&buffer, 0, directive_marker, &marker_offset);
		
    }
	
    return directive_values;
}

internal void modules_push_new(ModuleList *modules, Module *module)
{
    module->next = &nil_module;
    module->prev = &nil_module;
	
    if(is_nil_module(modules->root))
    {
		modules->root = module;
    }
    else
    {
		module->prev           = modules->current;
		modules->current->next = module;
    }
	
    modules->current = module;
    modules->count++;
}

#if 0

#define noinline __attribute__((noinline))

internal noinline void
crash_inner(int *volatile *slot)
{
    **slot = 42;
}

internal noinline void
crash_outer(void)
{
    int *volatile pointer = 0;
    crash_inner(&pointer);
}

#endif


internal String8
metaprogram(String8 buffer) { return buffer; }

internal String8 build_path(Arena *arena, String8 source_dir, String8 name, b32 relative)
{
    String8 result = PushString8(arena, source_dir.size + name.size);
    result.size = 0;
	
    if(relative) {
		source_dir = str8("./");
    }
	
    result = str8_insert(arena, source_dir, name, source_dir.size);
    source_dir = str8_concat(arena, source_dir, str8("/"));
    return result;
}

internal b32 handle_imports(Arena *arena, String8 buffer, String8List *out)
{
    String8 directive_marker        = str8("#");
    String8 directive_keyword       = str8("import");
	String8List *result = handle_directives(arena, buffer,  directive_marker, directive_keyword);
	if(result->count != 0) return false;
	out = result;
	return true;
}

internal String8 get_directory_from_path(Arena *arena, String8 path)
{
    String8 result = PushString8(arena, path.size);
    result.size = 0;
    for(u64 index = path.size; index > 0; --index)
    {
		if(cstring_is_slash(path.data[index - 1]))
		{
			u64 slash_index = index;
			result = str8_chop_right(path, path.size - (slash_index ? slash_index : 1));
			break;
		}
    }
	
    return result;
}

internal void str8_concat_list(String8List *a, String8List *b)
{
	b->last   = a->last->next;
	b->count  += a->count;
}

internal void handle_imports_and_push_to_modules(Arena *arena, ModuleList *list, String8 buffer)
{
	
	String8List *total_list = PushStructZero(arena, String8List);
	b32 found_one = true;
	while(found_one)
	{
		String8List* out = PushStructZero(arena, String8List);
		found_one = handle_imports(arena, buffer, out);
		if(!found_one) break;
		str8_concat_list(total_list, out);
	}
	
	FOR_STR8_NODE(total_list, current)
	{
		Module *new_module = PushStructZero(arena, Module);
		new_module->file_name = current->value;
		modules_push_new(list, new_module);
	}
}


//- ENTRYPOINT

ENTRYPOINT(argc, argv)
{
    Arena *arena = ArenaCreate(MiB(64));
    AssertLog(arena, "Failed to allocate memory");
	
    LogBacktrace;
	
    String8     source_path          = {};
    ModuleList  *modules             = PushStruct(arena, ModuleList);
    ModuleList  *module_list         = PushStruct(arena, ModuleList);
	
	
    if(argc <= 1)
    {
		Log("No parameters passed. The compiler is going to find the entry point and take that as the target");
        Log("help [--source=] \"example.jr\" [--include=] [--output=] example[.exe]]\n");
		Assert(0);
    }
    else
    {
		ScratchArena scratch = scratch_start(arena);
        for(u64 argument_index = 1; argument_index < (u64)argc; ++argument_index)
		{
			String8 argument = str8(argv[argument_index]);
            if(str8_starts_with(argument, str8("--include=")) || str8_starts_with(argument, str8("-I=")))
			{
                String8 include_path = consume_param(scratch.arena, argument);
                if(!str8_is_empty(&include_path))
				{
					Module *module = PushStructZero(arena, Module);
					String8 file = consume_param(arena, argument);
					str8_copy(&module->file_name, &file);
					modules_push_new(modules, module);
                }
            }
			else if(str8_starts_with(argument, str8("--source=")))
			{
				source_path = consume_param(scratch.arena, argument);
            }
			else if(str8_starts_with(argument, str8("--output=")))
			{
				// TODO: handle output file
				
			}
        }
		scratch_end(&scratch);
    }
	
	
	
	
    // NOTE: handle the target module, when not doing a directory scan this is usefull because we chase imports and the first import we handle is the target.
    // and after wards we get the rest
    if(source_path.size != 0)
    {
		Module *new_module = PushStruct(arena, Module);
		String8 module_name = str8_chop_left(source_path, (source_path.size + 1));
		new_module->file_name = PushString8(arena, module_name.size);
		new_module->file_name.size = 0;
		str8_copy(&new_module->file_name, &module_name);
		// NOTE: we have no owner because we are target
		new_module->file_owner = &nil_module;
		modules_push_new(modules, new_module);
    }
    else
    {
		// TODO(PLATFORM): recursion
		String8List *files = PushStructZero(arena, String8List);
		files = OpenDir(arena, &source_path);
		FOR_STR8_NODE(files, file_node)
		{
			// NOTE: no need to check if the file "exists" thats only needed when importing modules
			String8 extension = str8_get_file_extension(file_node->value);
			if(str8_is_match(str8(".jr"), extension))
			{
				Module *new_module = PushStructZero(arena, Module);
				new_module->file_name = file_node->value;
				modules_push_new(modules, new_module);
			}
		}
    }
	
	String8List *imported_file_names = PushArrayZero(arena, String8List, 32);
	String8 dir = get_directory_from_path(arena, str8(argv[0]));
	do {
		if(modules->root->buffer.size != 0) {
			handle_imports_and_push_to_modules(arena, modules, modules->root->buffer);
		}
		b32 import_cycle_found = detect_import_cycle_in_modules(modules);
#if 0
		Assert(import_cycle_found);
#endif
		
		if(imported_file_names->count == 0) break;
		FOR_STR8_NODE(imported_file_names, file)
		{
			file = PushStructZero(arena, String8Node);
			Module *new_module = PushStruct(arena, Module);
			String8 path = build_path(arena, dir, file->value, false);
			b32 file_exists = FileExists(arena, path);
			Assert(file_exists);
			new_module->file_name = file->value;
			modules_push_new(modules, new_module);
		}
		
	} while(imported_file_names->count != 0);
	
	
	
    for(Module *module = modules->root; !is_nil_module(module); module = module->next)
    {
		// TODO: lnx / win abstraction
		if(module->buffer.size == 0)
		{
			String8 dir = get_directory_from_path(arena, str8(argv[0]));
			String8 path = build_path(arena, dir, module->file_name, false);
			module->buffer = FileLoad(arena, path);
			AssertLog(module->buffer.size, "Failed to load module: " Str8Fmt "\nbuffer size: %d", Str8Arg(module->file_name), module->buffer.size);
		}
		
		// TODO: integrate some default compile time metaprogramming
		module->buffer = metaprogram(module->buffer);
		module->buffer = str8_cut_match(arena,  module->buffer,  str8("//"),  str8("\n"));
		module->buffer = str8_cut_match(arena,  module->buffer,  str8 ("/*"),  str8("*/"));
	}
	
	u64 count                    = modules->count;
	Thread *module_threads       = PushArrayZero(arena, Thread, count);
	LexArg *args                 = PushArrayZero(arena, LexArg, count);
	
	u64 thread_idx = 0;
	for(Module *module = modules->root; !is_nil_module(module); module = module->next, ++thread_idx)
	{
		Assert(args);
		args[thread_idx].arena     = ArenaCreate(MiB(64));
		args[thread_idx].buffer    = module->buffer;
		args[thread_idx].result    = PushStructZero(arena, TokenList);
		module_threads[thread_idx] = thread_launch(arena, mt_lexer_tokenize,  &args[thread_idx]);
	}
	
	
	thread_idx = 0;
	for(Module *module = modules->root; !is_nil_module(module->next); module = module->next, ++thread_idx)
	{
		b32 ok = thread_join(module_threads[thread_idx]);
		Assert(ok);
		module->token_list = args[thread_idx].result;
		
#ifdef THREADING
		module->token_list = lexer_tokenize(arena, module->buffer);
#endif
	}
	
	return 0;
	
}
