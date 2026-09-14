#include "base/base_include.h"

#include "messages.h"
#include "lexer.h"
#include "parser.h"
#include "preproc.h"
#include "parse_table.h"

#include "lexer.cpp"
#include "parser.cpp"
#include "preproc.cpp"

ENTRYPOINT()
{
    String8 path;
    if(count > 0) {
        path = str8(args[1]);
    } else {
Log("No arguments passed to fajr");
        return -1;
    }
    
    MemArena *global_arena = ArenaCreate(MiB(100));
    String8 buffer = FileLoad(global_arena, path);
    TokenTree *tree = tokenize_tree_fmt(global_arena, &buffer);
    Ast *ast = parse_tokenized_tree_fmt(global_arena, tree);
    
    return 0;
}
