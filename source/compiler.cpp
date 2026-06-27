// compiler

//- base stuff
// #define BASE_LOGGING
#define BASE_INCLUDE
#define BASE_IMPLEMENTATION
#include "base/base_include.h"

//- compiler includes
#include "lexer.h"
#include "parser.h"

//- entry point
int main(int argc, char **argv) {

#ifdef BASE_LOGGING
    printf("nil_token_node address: %p\n", &nil_token_node);
    printf("next pointer: %p\n", nil_token_node.next);
    printf("Are they equal? %d\n", nil_token_node.next == &nil_token_node);
#endif

    String8 path = {0};

    if(argc > 0) {
        path = ToString8((u8 *)&argv[1], sizeof(&argv[1]));

    } else {
        // no input file
        // @TODO: Logging
        return 1;
    }

    MemArena *global_arena = ArenaCreate(MiB(100));

    fill_in_nil_token_node(&nil_token_node); // @debugging: this can be removed this was for testing the read only section with msvc

    // read the file into a buffer
    String8 buffer =  FileLoad(global_arena, path.data);

    // parse the buffer into a token tree

    breakpoint();
    TokenTree *tree =  lex(global_arena, &path);
    Ast* ast = parse(global_arena, tree);



    return 0;
}