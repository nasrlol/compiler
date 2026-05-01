#define BASE_INCLUDE
#include "base/base_include.h"

#include "lexer/lexer.h"
#include "parser/parser.h"

#include "lexer/lexer.c"
#include "parser/parser.c"

int
main(int argc, char **argv)
{
    unused(argc);
    unused(argv);

    const char *path = "../tests/main.fajr";
    string8.data buffer = file_read(path);

    return 0;
}
