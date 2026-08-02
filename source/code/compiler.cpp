#include "base/base_include.h"

#include "lexer.h"
#include "parser.h"

#include "lexer.cpp"
#include "parser.cpp"

 int main(int argc, char **argv)
    {
    String8 path;

    if(argc > 0) path = str8(argv[1]);
    else return 1;

    MemArena *global_arena = ArenaCreate(MiB(100));


    breakpoint();
String8 buffer = FileLoad(global_arena, path);
Log("buffer ");
Log(Str8Fmt, Str8Arg(buffer));

    TokenTree *tree = tokenize_tree_fmt(global_arena, &path);
    Ast *ast = parse_tokenized_tree_fmt(global_arena, tree);

    return 0;
}