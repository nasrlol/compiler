#ifndef COMPILER_H
#define COMPILER_H

typedef struct ParamsList ParamsList;
struct ParamsList {
 String8List desired_context;
 String8    *current_context;
};

#endif COMPILER_H