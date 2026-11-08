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
internal T *peek_forward_rec(T *node, s32 count) {
	if(count > 0) return peek_forward_rec(node->next, count - 1);
	return node;
}

internal b32 is_nil_module(Module *it) {return (it != NULL || it == &nil_module);}

internal b32 detect_import_cycle_in_modules(ModuleList *lst) {
	for(Module *fast = lst->current; !is_nil_module(fast) && !is_nil_module(fast->next) ; fast = peek_forward_rec(fast, 1))
	{
		for(Module *slow = lst->current; !is_nil_module(slow) && is_nil_module(slow->next->next); slow = peek_forward_rec(slow, 2)) {
			return false;
		}
	}
	return false;
}

internal String8 consume_param(String8 param)
{
	u64 equal_position              = 0;
	b32 found_equal_sign            = Str8FindNeedle(&param, 0, "=", &equal_position);
	if(found_equal_sign) param      = str8_chop_left(param, equal_position + 1);
	else {
		Log("wrong compiler param");
		return str8_zero();
	}
	return param;
}

internal String8 get_word(Arena *arena, String8 buffer, String8 delimiter)
{
	u64 offset = 0;
	b32 reached = false;
	
	for(u32 idx = 0; (!reached) && (idx < buffer.size); ++idx, ++buffer.data) {
		reached = str8_is_match(buffer, delimiter);
		offset = idx;
	}
	
	buffer.data -= offset;
	
	if(offset != 0) {
		String8 result = PushString8(arena, offset);
		result = str8_create_substr(&buffer, 0, offset);
		return result;
	}
	
	return str8_zero();
}

internal String8 get_word_nl(Arena* arena, String8 buffer) { return get_word(arena, buffer, str8(" ")); }

typedef struct BufferSection BufferSection;
struct BufferSection {
	u64 start;
	u64 end;
};

// NOTE: we return a list a of  all pound sign values  that we found  within the current bufferr
// when find any 1..n we return the next tokens in a String8List.
internal String8List *handle_directives(Arena *arena, String8 buffer, String8 seperator, String8 keyword)
{
	Assert(buffer.size);
	String8List *list = PushStruct(arena, String8List);
	
	b32 found = false;
	u64 offset = 0;
	found = str8_find(&buffer, 0, seperator, &offset);
	
	while(found)
	{
		// TODO: think about how we delete the keywords because they should get handled here
		buffer = str8_chop_left(buffer, offset + 1);
		
		String8 import_word = get_word_nl(arena, buffer);
		
		if(str8_is_match(import_word, keyword)) {
			while (cstring_is_whitespace(*buffer.data)) { buffer = str8_chop_left(buffer, 1); }
			String8 value = get_word_nl(arena, buffer);
			String8Node *node = PushStruct(arena, String8Node);
			*node = ToString8Node(value);
			str8_list_push_node(list, node);
		} else if(str8_is_empty(&import_word)) {
			Log("Error:" Str8Fmt, Str8Arg(import_word));
			PANIC("Pound sign was found but no directive defined.");
		}
		
		found = str8_find(&buffer, 0, seperator, &offset);
	}
	
	return list;
}

internal void modules_push_new(ModuleList *modules, Module *module) {
	
	if(is_nil_module(modules->root)) {
		modules->root = module;
		modules->current = module;
	} else {
		// NOTE: this is assinged a nil module because there is no owener to the target file.
		// but within this approach there bassically is never a file owner
		if(modules->current != &nil_module) {
			modules->current = module;
			modules->current->next = &nil_module;
		} else {
			modules->current->next = module;
			modules->current->next->prev = modules->current;
			modules->current = module;
		}
		modules->count++;
	}
}

ENTRYPOINT(argc, argv)
{
    Arena *arena = ArenaCreate(MiB(256));
    AssertLog(arena, "Failed to allocate memory");
	
    String8List *context_directories  = PushStruct(arena, String8List);
    String8      target     = PushString8(arena, 0);
    ModuleList  *modules              = PushStruct(arena, ModuleList);
    ModuleList  *module_list          = PushStruct(arena, ModuleList);
	
    if(argc <= 1) {
		Log("No parameters passed. The compiler is going to find the entry point and take that as the target");
        Log("help [--source=] \"example.jr\" [--include=] [--output=] example[.exe]]\n");
    } else {
        // NOTE: we are bigger then one arguments so we can start assuming that a source file was passed
        for(u64 argument_index = 1; argument_index < (u64)argc; ++argument_index) {
			String8 a = str8(argv[argument_index]);
            if(str8_starts_with(a, str8("--include=")) || str8_starts_with(a, str8("-I="))) {
                String8 include = consume_param(a);
                if(!str8_is_match(include, str8_zero())) {
                    String8Node include_node = ToString8Node(include);
                    str8_list_push_node(context_directories, &include_node);
                }
            } else if(str8_starts_with(a, str8("--source="))) {
				target = consume_param(a);
            }
        }
    }
	
	
    // the directory of the input file: everything before the last slash, "." if there is none
    String8 context_path = {};
	for(u64 index = target.size; index > 0; --index) {
        if(cstring_is_slash(target.data[index - 1])) {
            u64 slash_pos = index - 1;
			// NOTE: this check makes sure we remove the last trailing slash in case we want to trim by 0
            context_path  = str8_chop_right(target, target.size - (slash_pos ? slash_pos : 1));
            break;
        }
    }
	
    Log("context directory: " Str8Fmt, Str8Arg(context_path));
	
	
    String8List *files = PushStruct(arena, String8List);
	Module *new_module = PushStruct(arena, Module);
	new_module->name   = PushString8(arena, target.size);
    if(target.size != 0)
    {
		//  TODO: doing a +1 to handle the trailing slash. this needs to be fixed from the root
		String8 name = str8_chop_left(target, (context_path.size + 1));
		str8_copy(&new_module->name, &name);
		// NOTE: we have no owner because we are target
		new_module->owner = &nil_module;
    }
    else
    {
		// TODO: find the files recursivly  first check  if the file is a folder
		// if the file is a folder open the folder and read everything in there
		
		// NOTE: no file should have an owner i think
        files = DirLoadFileNames(arena, &context_path); // TODO: this should be replaced with a recursive function
        FOR_STR8_NODE(files, current) {
            String8 extension = str8_get_file_extension(current->value);
            if(str8_is_match(str8(".jr"), extension)) {
                new_module->name = current->value;
				
				modules_push_new(modules, new_module);
			}
			
		}
	}
	
	
	// TODO: iterate on the modules and load theyre files in parallel
	// afterwards start doing the rest of  the  stuff
	{
		
		// the first buffer should acutally be tha main target buffer so get the name
		// get the path and then load it in
		// after you did all of the proper preprocessing etc.
		{
			// make sure we are starting with a clena slate
			Assert(modules->current == &nil_module || modules->current == NULL);
			modules->current = PushStruct(arena, Module);
			modules->current->buffer = FileLoad(arena, target);
			modules->root = modules->current;
			
			String8 seperator = str8("#");
			String8 keyword   = str8("import");
			
			// handle comments
			{
				modules->current->buffer = str8_cut_match(arena,  modules->current->buffer,  str8("//"),  str8("\n"));
				modules->current->buffer = str8_cut_match(arena,  modules->current->buffer, str8("/*"),  str8("*/"));
			}
			
			
			String8List *to_import_file_names =  handle_directives(arena, modules->current->buffer,  seperator, keyword);
			
			if(to_import_file_names->count == 0) {
				// TODO: exit in someway
				// NOTE: no import handling needed and we only have one module
			}
			
			// TODO:  build and  assign the relative path based on the name and save that in the module struct
			// this maybe based on imports
			FOR_STR8_NODE(to_import_file_names, c)
			{
				// TODO: push a new modules with the relative path
				str8_insert(arena, modules->current->relative_path, str8("./"), 0);
				b32 exists = FileExists(arena, modules->current->relative_path);
				AssertLog(exists, "failed to import: " Str8Fmt, Str8Arg(modules->current->buffer));
				Module *new_module = PushStruct(arena, Module);
				new_module->name = c->value;
				str8_append(&new_module->relative_path, c->value);
				modules_push_new(modules, new_module);
			}
			
			
			
		}
		
		FOR_NODE(m, modules, &nil_module) {
			String8 buffer =  FileLoad(arena, m->relative_path);
			
			if(!buffer.data){Assert(0);}
			str8_copy(&m->buffer, &buffer);
			m->tree =  tokenize_tree(arena, m->buffer);
			AssertLog(m->tree, "Failed to create Token Tree");
			
		}
	}
	
	return 0;
	
}