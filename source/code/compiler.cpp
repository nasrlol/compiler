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

ENTRYPOINT()
{

 Log("\n\nCurrent directory: %s\n", args[0]);
 String8 path;
 if (count <= 1) {
  path = str8("./tests/main.jr");
  Log(Str8Fmt " is the default tests file received\n", Str8Arg(path));
 } else {
  Log("Received file %s\n", args[1]);
  path = str8(args[1]);
 }

 MemArena *arena = ArenaCreate(MiB(256));
 AssertLog(arena, "Failed to allocate memory\n");

 String8 buffer = FileLoad(arena, path);
 scan_buffer_and_handle_modules(arena, &buffer);

 TokenTree *tree = tokenize_tree_fmt(arena, &buffer);
 if (!tree)
  Log("Failed to create Token Tree\n");

 Ast *ast = parse_tokenized_tree_fmt(arena, tree);
 if (!ast)
  Log("Failed to create AST\n");

 return 0;
}
