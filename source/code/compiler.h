#ifndef COMPILER_H
#define COMPILER_H

global_variable b32 import_chasing = false;

typedef struct Module Module;
struct Module {
	TokenList *token_list;
	String8    file_name;
	Module    *next;
	Module    *prev;
	Module    *file_owner;
	String8   buffer;
	
};

typedef struct ModuleList ModuleList;
struct ModuleList {
	Module *current;
	Module *root;
	u64 count;
};

ReadOnly Module nil_module = {
	.token_list  = &nil_token_list,
	.next  = &nil_module,
	.prev  = &nil_module,
	.file_owner = &nil_module,
};

#endif // COMPILER_H