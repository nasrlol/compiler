#ifndef COMPILER_H
#define COMPILER_H

global_variable b32 import_chasing = false;

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

template <typename T>
internal T       *peek_forward_rec(T *node, s32 count);
internal b32      is_nil_module(Module *it);
internal b32      detect_import_cycle_in_modules(ModuleList *lst);
internal String8  consume_param(String8 param);
internal String8  get_word(Arena *arena, String8 buffer, String8 delimiter);
internal String8  get_word_nl(Arena *arena, String8 buffer);
internal String8List *handle_directives(Arena *arena, String8 buffer, String8 seperator, String8 keyword);

#endif // COMPILER_H