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
    String8 context_path = str8("./tests");
    for(u64 index = main_target_path.size; index > 0; --index) {
        if(cstring_is_slash(main_target_path.data[index - 1])) {
            u64 slash_pos = index - 1;
            context_path  = str8_chop_right(main_target_path, main_target_path.size - (slash_pos ? slash_pos : 1)); // NOTE: this check makes sure we remove the last trailing slash in case we want to trim by 0
            break;
        }
    }

    Log("context directory: " Str8Fmt, Str8Arg(context_path));


    String8 main_target_buffer = PushString8(arena, 0);
    String8List *files = PushStruct(arena, String8List);
    if(main_target_path.size != 0)
    {
        u64 insertion_position     = 0;

        main_target_buffer = FileLoad(arena, main_target_path);
        main_target_buffer = handle_comments(arena, main_target_buffer);

        String8List *directives    = handle_directives(arena, main_target_buffer, str8("#"));
        if(directives->count == 0) goto preproc_end;
    }
    else
    {
        files = DirLoadFileNames(arena, &context_path);
        FOR_STR8_NODE(files, current) {
            String8 extension = str8_get_file_extension(current->value);
            if(str8_is_match(str8(".jr"), extension))
            {
                Module *new_module = &nil_module;
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


if(modules->count == 0)
{
AssertLog(0, Str8Fmt "No source files found." );
        main_target_buffer = handle_all_modules_into_buffer(arena, modules);
    }


    preproc_end
:

    // produce a preprocrssor intermediate representation
    if(FileExists(arena, str8("output.jr.ps"))) {
        Log("Deleting previous output file");
        FileWrite(str8("placeholder"), str8("output.jr.ps"));
    }
    Log("Creating new intermediate debug file");
    if(1) {
        b32 debug_output_result = FileWrite(main_target_buffer, str8("output.jr.ps"));
        AssertLog(debug_output_result, "Failed to fill in the buffer");
    }

    // NOTE: this means there has been no attempt to import any external modules
    if(module_list->count != 0){
        for(Module *current = modules->current; current->next != &nil_module && current->next != NULL;
            current = current->next) {
            String8 module_buffer = FileLoad(arena, main_target_path);
            current->tree         = tokenize_tree(arena, module_buffer);
        }
    }

    //- start the lexing and parsing
    TokenTree *tree = tokenize_tree(arena, main_target_buffer);
    AssertLog(tree, "Failed to create Token Tree");
    Ast *ast        = parse_tokenized_tree(arena, tree);
    AssertLog(ast, "Failed to create AST");

    return 0;

}