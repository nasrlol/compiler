// compiler

//- base stuff
#define BASE_INCLUDE
#define BASE_IMPLEMENTATION
#include "base/base_include.h"

//- compiler includes
#include "lexer.h"
#include "parser.h"

//- entry point
int main(int argc, char **argv) {
 String8 path = {0};

 if(argc > 0) {
  path = ToString8((u8 *)&argv[1], sizeof(&argv[1]));

 } else {
  // no input file
  // TODO: Logging
  return 1;
 }

 MemArena *global_arena = ArenaCreate(MiB(100));

 // read the file into a buffer
 // String8 buffer =  FileLoad(global_arena, path.data);

 // parse the buffer into a token tree

 TokenTree *tree =  lex(global_arena, &path);
 // Ast* ast = parse(global_arena, *tree);



 return 0;
}