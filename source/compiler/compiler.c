#define BASE_INCLUDE
#define BASE_IMPLEMENTATION
#include "base/base_include.h"

#include "lexer.h"
#include "lexer.c"

#include "parser.h"
#include "parser.c"


int main(int argc, char **argv) {
  string8 path = {0};

  if(argc > 0) {
    path = String8(&argv[1], sizeof(&argv[1]));
  } else {
    // no input file
    // TODO: Logging
    return 1;
  }

  mem_arena *global_arena = ArenaCreate(MiB(100));

  // read the file into a bfufer
  string8 buffer =  FileLoad(global_arena, path);

  // parse the buffer into a token tree

  token_tree *tt =  lex(global_arena, &path);


  return 0;
}
