#define DBEUG
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
    if(global_arena)
    {

ScratchArena inital_buffer_scratch = scratch_start(global_arena);
        String8 buffer = FileLoad(global_arena, path);

// NOTE: pass the buffer. but internally allocate a big enough one because we don't know how much
// memory we need. we basically will always need more than the already allocatred buffer
// NOTE: the  scratch arena gets closed  inside the scan buffer handler
// TODO: is this even a good idea?
scan_buffer_handle_modules(global_arena, &inital_buffer_scratch,  &buffer);
        TokenTree *tree = tokenize_tree_fmt(global_arena, &buffer);
        if(tree) {
            Ast *ast = parse_tokenized_tree_fmt(global_arena, tree);
            if(!ast) {
                Log("Failed to create AST\n");
                return 1;
            }
        } else {
            Log("Failed to create Token Tree\n");
            return 1;
        }
    } else  {
        Log("Failed to allocate memory\n");
        return 1;
    }

    return 0;
}
