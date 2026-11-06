#define DBEUG 1
#include "base/base_include.h"
#include "lexer.h"
#include "messages.h"
#include "parse_table.h"
#include "parser.h"
#include "preproc.h"
#include "lexer.cpp"
#include "parser.cpp"
#include "preproc.cpp"
#include "base/base_raddbg.h"

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
ENTRYPOINT(argc, argv)
{
    Arena *arena = ArenaCreate(MiB(256));
    AssertLog(arena, "Failed to allocate memory");
	
    String8List *context_directories  = PushStruct(arena, String8List);
    String8      main_target_path     = PushString8(arena, 0);
    ModuleList  *modules              = PushStruct(arena, ModuleList);
    ModuleList  *module_list          = PushStruct(arena, ModuleList);
	
    if(argc <= 1) {
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
                main_target_path = consume_param(a);
            }
        }
    }
	
	
    // the directory of the input file: everything before the last slash, "." if there is none
    String8 context_path = {};
	for(u64 index = main_target_path.size; index > 0; --index) {
        if(cstring_is_slash(main_target_path.data[index - 1])) {
            u64 slash_pos = index - 1;
			// NOTE: this check makes sure we remove the last trailing slash in case we want to trim by 0
            context_path  = str8_chop_right(main_target_path, main_target_path.size - (slash_pos ? slash_pos : 1));
            break;
        }
    }
	
    Log("context directory: " Str8Fmt, Str8Arg(context_path));
	
	
    String8 main_target_buffer = PushString8(arena, 0);
    String8List *files = PushStruct(arena, String8List);
	Module *new_module = PushStruct(arena, Module);
	new_module->name   = PushString8(arena, main_target_path.size);
    if(main_target_path.size != 0)
    {
		//  TODO: doing a +1 to handle the trailing slash. this needs to be fixed from the root
		String8 name = str8_chop_left(main_target_path, (context_path.size + 1));
		str8_copy(&new_module->name, &name);
		// NOTE: we have no owner because we are target
		new_module->owner = &nil_module;
    }
    else
    {
		// TODO: find the files recursivly  first check  if the file is a folder
		// if the file is a folder open the folder and read everything in there
		
		// NOTE: no file should have an owner i think
        files = DirLoadFileNames(arena, &context_path);
        FOR_STR8_NODE(files, current) {
            String8 extension = str8_get_file_extension(current->value);
            if(str8_is_match(str8(".jr"), extension))
            {
                new_module->name = current->value;
				// NOTE: this is assinged a nil module because there is no owener to the target file.
				// but within this approach there bassically is never a file owner
                if(modules->current != &nil_module) {
                    modules->current = new_module;
                    modules->current->next = &nil_module;
                } else {
					modules->current->next = new_module;
					modules->current->next->prev = modules->current;
					modules->current = new_module;
				}
				modules->count++;
            }
		}
	}
	
	
	// TODO:  build and  assign the relative path based on the name and save that in the module struct
	// this maybe based on imports
	{
		
		
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
			modules->current = FileLoad(arena, main_target_path);
			String8List *directives    = handle_imports(arena, main_target_buffer, str8("#"));
			
		}
		
		FOR_NODE(mo, modules, &nil_module) {
			String8 buffer =  FileLoad(arena, mo->relative_path);
			str8_copy(&mo->buffer, &buffer);
			handle_comments(arena, mo->buffer);
		}
		
	}
	Assert(FileWrite(main_target_buffer, str8("output.jr.ps")));
	
    TokenTree *tree = tokenize_tree(arena, main_target_buffer);
    AssertLog(tree, "Failed to create Token Tree");
    Ast *ast        = parse_tokenized_tree(arena, tree);
    AssertLog(ast, "Failed to create AST");
	
    return 0;
	
}