#define DEV
#include "base/base_include.h"

//- compiler includes
#include "lexer.h"
#include "parser.h"


typedef struct DebugContextInfo DebugContextInfo;
struct DebugContextInfo
{
 String8 current_path;
 String8 path;
 String8 file;
 };

 internal DebugContextInfo
 gather_context_information_for_debugging() {
 // TODO: improve the logging when the user passes bad input
 return {};
 }


 //- entry point
 int
 main(int argc, char **argv)
 {
 Log("nil_token_node address: %p\n", &nil_token_node);
 Log("next pointer: %p\n", nil_token_node.next);
 Log("Are they equal? %d\n", nil_token_node.next == &nil_token_node);

 String8 path = {0};

 if(argc > 0)
 {
 path = ToString8((u8 *)&argv[1], sizeof(&argv[1]));
 }
 else
 {
 // no input file
 // TODO: Logging
 return 1;
 }

 MemArena *global_arena = ArenaCreate(MiB(100));

 fill_in_nil_token_node(&nil_token_node); // TODO: this can be removed this was for testing the read only section with msvc

 // read the file into a buffer
 String8 buffer = FileLoad(global_arena, path);

 // parse the buffer into a token tree

 breakpoint();
 TokenTree *tree = lex(global_arena, &path);
 Ast       *ast  = parse(global_arena, tree);

 return 0;
}