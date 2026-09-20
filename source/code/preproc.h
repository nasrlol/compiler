#ifndef PREPROC_H
#define PREPROC_H

typedef struct Module Module;
struct Module
{
    TokenTree *tree;
    String8 name;
      void *handle; // TODO: update the platform library to support a generic file type
};


#endif // PREPROC_H
