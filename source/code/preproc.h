#ifndef PREPROC_H
#define PREPROC_H

typedef struct Module Module;
struct Module {
	TokenTree *tree;
	String8    name;
	String8    relative_path;
	Module    *next;
	Module    *prev;
	Module    *owner;
	String8   buffer;
};

typedef struct ModuleList ModuleList;
struct ModuleList {
	Module *current;
	Module *root;
	u64 count;
};

ReadOnly Module nil_module = {
	.tree  = &nil_token_tree,
	.next  = &nil_module,
	.prev  = &nil_module,
	.owner = &nil_module,
};

internal Token *consume_token_from_buffer(Arena *arena, String8 *buffer);
internal ModuleList *scan_buffer_and_handle_modules(Arena *arena, String8 *buffer, String8 context_directory, b32 import_chasing);

#endif // PREPROC_H
