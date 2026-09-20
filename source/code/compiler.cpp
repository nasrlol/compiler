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


internal void tests() {

MemArena *arena = ArenaCreate(MiB(1));
String8 buffer = PushString8(arena, 100);
String8 hw = str8("Hello World\n");
  str8_copy(&buffer, &hw);
str8_append(&buffer, str8("Assalamoealaykoem Warahmatoellahi Wabarakatoeh\n"));
LogStr8(buffer);

}

ENTRYPOINT() {

tests();

    String8 path;
    if(count < 0) {
Log("No arguments passed to fajr");
return -1;
}
        path = str8(args[1]);

    MemArena *global_arena = ArenaCreate(MiB(200));
    if(!global_arena) Log("Failed to allocate memory\n");

        String8 buffer = FileLoad(global_arena, path);
scan_buffer_and_handle_modules(global_arena, &buffer);

        TokenTree *tree = tokenize_tree_fmt(global_arena, &buffer);
        if(!tree) Log("Failed to create Token Tree\n");

            Ast *ast = parse_tokenized_tree_fmt(global_arena, tree);
            if(!ast) Log("Failed to create AST\n");

    return 0;
}
