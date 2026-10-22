#ifndef PREPROC_H
#define PREPROC_H


typedef struct Module Module;
struct Module {
 TokenTree *tree;
 String8    name;
 Module    *next;
 Module    *prev;
 };

 typedef struct ModuleList ModuleList;
 struct ModuleList {
 Module *current;
 };

 ReadOnly Module nil_module = {
 &nil_token_tree,
 {},
 &nil_module,

};

internal Token      *consume_token_from_buffer(MemArena *arena, String8 *buffer);
internal ModuleList *scan_buffer_and_handle_modules(MemArena *arena, String8 *buffer, String8 context_directory, b32 import_chasing);

#endif // PREPROC_H
