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

// returns the value fter the equal sign
internal String8 handle_compiler_param(char *argv) {

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

 // Global compiler params
 String8List *context_directories = PushStruct(arena, String8List);
 b32 import_chasing = false;
 String8 main_target_path = PushString8(arena, 0);

 ModuleList *modules = PushStruct(arena, ModuleList);
 ModuleList *module_list = PushStruct(arena, ModuleList);

 if (argc <= 1) {
  main_target_path = str8("./tests/main.jr");
  Log("loaded: " Str8Fmt, Str8Arg(main_target_path));
  import_chasing = true;
 } else {
  Log("Received file %s", args[1]);
  main_target_path = str8(args[1]);
  for(u64 argument_index = 1; argument_index < (u64)argc; ++argument_index) {
   cstring a = args[argument_index];
   // NOTE: Handles include directories
   if(CstringIsMatch(a, "--include=") || CstringIsMatch(a, "-I=")) {
    String8 include = handle_compiler_param(a);
    if(!str8_is_match(include, str8_zero())) {
     String8Node include_node = ToString8Node(include);
     str8_list_push_node(context_directories, &include_node);
    }
    import_chasing = false;
   } else if (CstringIsMatch(a, "--chase-imports=") || CstringIsMatch(a, "-C")) { import_chasing = true; }
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

 String8List *files = NULL;
 String8 total_files_buffer = {};

 if(import_chasing) {
  files = DirLoad(arena, &context_path);
  total_files_buffer = PushString8(arena, 0);

  str8_append(&total_files_buffer, str8("[ "));

  FOR_STR8_NODE(files, current) {
   String8 extension = str8_get_file_extension(current->value);
   if(str8_is_match(str8(".jr"), extension))
   {
    str8_append(&total_files_buffer, current->value);
    str8_append(&total_files_buffer, str8(" "));
   } else {
    Log(Str8Fmt " does not contain the extension jr" , Str8Arg(current->value));
   }
  }

  str8_append(&total_files_buffer, str8("]"));

 } else {

  // We load the target file, and based on what is included in that target file. we chase the other imports.

  String8 entry_file_buffer = FileLoad(arena, main_target_path);
  Log("Files in the current directory: " Str8Fmt, Str8Arg(total_files_buffer));


  u64 insertion_position = 0;

  Token *token = consume_token_from_buffer(arena, &entry_file_buffer);
  if(is_token_nil(token)) {
   Log("Consuming a token from the buffer returned a nil token");
  }

  // TODO: auto per directory include module feature
  // compile time arguments

  // TODO: remove one of these in the future when we have a proper seperation between the directory and per file loading

  String8 included_module_buffer = {};
  if (!is_token_nil(token)) {

   Module *module = PushStruct(arena, Module);
   included_module_buffer = FileLoad(arena, token->value.as_str);
   if(included_module_buffer.size == 0) {
    Log("included module buffer size is 0");
    PANIC("include not found in current directory.\n");
   }

   module->name = token->value.as_str;

  } else {
   // NOTE: if it's a nil token we should be able to assume that there is no pound token in the
   // file or that the directive used isnt known. or some other edge case. this will be logged
   // from inside the consume_token_from_buffer function
   Log("preprocessor early exit, no file specified");
   OSExit(0);
  }

  if(import_chasing){

   // TODO: this is the seperation between the per directoryu and per file handling
   // disabled for now
   // TODO: also needs a bit of refactoring
   String8 temp = {};

   FOR_STR8_NODE(files, current)
   {
    if(str8_is_match(current->value, token->value.as_str)) {

     String8 current_value_file_path = current->value;

     if (!temp.size) {
      temp= FileLoad(arena, current_value_file_path);
     } else {
      str8_append(&temp, FileLoad(arena, current_value_file_path));
     }
     temp= FileLoad(arena, current->value);
     b32 result = str8_insert(arena, &temp, &entry_file_buffer, 0);
     if(result) {
      Log("preprocessor handled " Str8Fmt, current_value_file_path);

     } else {
      break;
      String8List *files = DirLoad(arena, &current_value_file_path);

      Log("all files in current directory");
      FOR_STR8_NODE(files, current) {
       Log("files available in the current directory");
       LogStr8(current->value);
      }
     }
    }


    {
     Module *new_module = &nil_module;
     new_module->name = token->value.as_str;
     if(module_list->current != &nil_module) {
      module_list->current = new_module;
      module_list->current->next    = &nil_module;
     } else {
      if(modules->current == &nil_module) module_list->current->next = new_module;
      ++module_list->count;
      module_list->current = new_module;
     }
    }
   }

  }

  insertion_position = 0;
  if(included_module_buffer.size > 0) {
   b32 inserted = str8_insert(arena, &entry_file_buffer, &included_module_buffer, insertion_position); // TODO: ii dont know on which buffer we are actually working
   AssertLog(inserted, "Failed to insert module buffer, even though it wasn't emptyh");

  }
 }

#if DEBUG

 if(FileExists(str8("output.jr.ps"))) {
  Log("deleting previous output file");
  FileWrite(str8(""), str8("output.jr.ps"));
 }

 Log("creating new intermediate debug file");
 if(1) { // TODO: this result needs to be retrieved from the insertion
  b32 debug_output_result = FileWrite(total_files_buffer, str8("output.jr.ps"));
  Assert(debug_output_result);
 }

#endif


 if (module_list->count == 0) {
  AssertLog(0, "no external libraries found");
 } else {
  for (Module *current = modules->current; current->next != &nil_module && current->next != NULL; current = current->next) {
   String8 module_buffer = FileLoad(arena, main_target_path);
   current->tree         = tokenize_tree_fmt(arena, &module_buffer);
  }
 }

 //- start the lexing and parsing
 TokenTree *tree = tokenize_tree_fmt(arena, &total_files_buffer);
 if (!tree)
  Log("Failed to create Token Tree");

 Ast *ast = parse_tokenized_tree_fmt(arena, tree);
 if (!ast)
  Log("Failed to create AST");

 return 0;
}