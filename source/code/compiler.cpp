#include "base/base_include.h"
#include "lexer.h"
#include "parser.h"

 int
    main(int argc, char **argv)
    {
    String8 path;

    if(argc > 0) path = str8(&argv[1]);
    else return 1;

    MemArena *global_arena = ArenaCreate(MiB(100));

    String8 buffer = FileLoad(global_arena, path);

    TokenTree *tree = lex(global_arena, &path);
    Ast       *ast  = parse(global_arena, tree);

    return 0;
}