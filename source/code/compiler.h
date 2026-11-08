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



#endif // COMPILER_H