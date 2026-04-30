#define BASE_UNITY
#include "base/base_include.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

#include "lexer/lexer.c"
#include "parser/parser.c"

int
main(int argc, char **argv)
{
    mem_arena *global = arena_create(GiB(1));
    if (argc < 2) {
        print("Usage: program <file>\n");
        return 1;
    }

    i32 fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        print("Error: Cannot open file\n");
        return 1;
    }

    u8 input[4096];
    u64 bytes_read;
    bytes_read = read(fd, input, sizeof(input));

    string8 buffer = {0};

    buffer.data = input;
    buffer.size = bytes_read;

    token_list *List = PushStruct(global, token_list);
    concrete_syntax_tree *Tree = PushStruct(global, concrete_syntax_tree);

    Lex(&buffer, global, List);
    Parse(global, List, Tree);

    close(fd);
    return 0;
}
