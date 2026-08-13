#ifndef PREPROC_H
#define PREPROC_H

typedef struct module_file_handle module_file_handle;
struct module_file_handle
{
String8 buffer;
};

typedef struct module module;
struct module
{
TokenTree *tree;
String8 name;
module_file_handle *handle;
};

internal module *create_module();
internal module_file_handle *find_module();

#endif // PREPROC_H