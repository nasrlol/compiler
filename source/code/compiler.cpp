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

// Returns the value after the equal sign
internal String8 consume_param(char *argv) {

 String8 param = str8(argv);
 u64 equal_position = 0;
 b32 found_equal_sign = Str8FindNeedle(&param, 0, "=", &equal_position); // assume that all params get prefixed with a equal sign
 if(found_equal_sign) {
  param = str8_chop_left(param, equal_position);
 } else {
  Log("wrong compiler param");
  return str8_zero();
 }

 return param;
}

ENTRYPOINT()
{

 MemArena *arena = ArenaCreate(MiB(256));
 AssertLog(arena, "Failed to allocate memory");

 b32          import_chasing       = false;
 String8List *context_directories = PushStruct(arena, String8List);
	String8      main_target_path     = PushString8(arena, 0);
	ModuleList  *modules              = PushStruct(arena, ModuleList);
	ModuleList  *module_list          = PushStruct(arena, ModuleList);

 if(argc <= 1) {
Log("help [--source=] \"example.jr\" [--include=] [--output=] example[.exe]]\n");

 } else {
// NOTE: we are bigger then one arguments so we can start assuming that a source file was passed
  for(u64 argument_index = 1; argument_index < (u64)argc; ++argument_index) {
   cstring a = args[argument_index];
   if(CstringIsMatch(a, "--include=") || CstringIsMatch(a, "-I=")) {
    String8 include = consume_param(a);
    if(!str8_is_match(include, str8_zero())) {
     String8Node include_node = ToString8Node(include);
     str8_list_push_node(context_directories, &include_node);
    }
}else if(CstringIsMatch(a, "--source=")) {
main_target_path = consume_param(a);
}
  }
 }


 // the directory of the input file: everything before the last slash, "." if there is none
 String8 context_path = str8(".");
 for(u64 index = main_target_path.size; index > 0; --index) {
  if (cstring_is_slash(main_target_path.data[index - 1])) {
   u64 slash_pos = index - 1;
   context_path  = str8_chop_right(main_target_path, main_target_path.size - (slash_pos ? slash_pos : 1)); // NOTE: this check makes sure we remove the last trailing slash in case we want to trim by 0
   break;
  }
 }

 Log("context directory: " Str8Fmt, Str8Arg(context_path));


 if(main_target_path.size != 0)
{
		string8 main_target_buffer = fileload(arena, main_target_path);
		u64 insertion_position     = 0;
		main_target_buffer         = handle_all_comments(main_target_buffer);
		string8list *directives    = handle_directives(arena, &main_target_buffer);
		if(directives.count == 0) goto preproc_end;
 }
 else
{

 String8List *files = PushStruct(arena, String8List);
		files = DirLoadFileNames(arena, &context_path);
		FOR_STR8_NODE(files, current) {
			String8 extension = str8_get_file_extension(current->value);
			if(str8_is_match(str8(".jr"), extension))
{
				Module *new_module = &nil_module;
				new_module->name = current->value;
				if(modules->current != &nil_module) {
					modules->current = new_module;
					modules->current->next    = &nil_module;
				} else if(modules->current == &nil_module) modules->current->next = new_module;
			}
		}
		else {
			Log(Str8Fmt "No source files found." , Str8Arg(current->value));
			Assert(0);
		}

		buffer = handle_all_modules_into_buffer(arena, modules);
		
}

preproc_end:
// PREPROCESSOR END
	////////////////////////////////


// produce a preprocrssor intermediate representation
 if(FileExists(str8("output.jr.ps"))) {
  Log("Deleting previous output file");
  FileWrite(str8("placeholder"), str8("output.jr.ps"));
 }
 Log("Creating new intermediate debug file");
 if(1) {
  b32 debug_output_result = FileWrite(buffer, str8("output.jr.ps"));
  AssertLog(debug_output_result, "Failed to fill in the buffer");
 }

 if (module_list->count != 0) {
  for(Module *current = modules->current; current->next != &nil_module && current->next != NULL;
 current = current->next) {
   String8 module_buffer = FileLoad(arena, main_target_path);
   current->tree         = tokenize_tree_fmt(arena, &module_buffer);
  }
 }

 //- start the lexing and parsing
 TokenTree *tree = tokenize_tree_fmt(arena, /*TODO: what buffer do we insert in?*/);
 if (!tree)
  Log("Failed to create Token Tree");

 Ast *ast = parse_tokenized_tree_fmt(arena, tree);
 if (!ast)
  Log("Failed to create AST");

 return 0;
}