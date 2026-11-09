#ifndef BASE_RADDBG_H
#define BASE_RADDBG_H

#if defined(DEBUG) && DEBUG && COMPILER_GCC || COMPILER_CLANG

#define RaddbgGlue_(a, b) a##b
#define RaddbgGlue(a, b)  RaddbgGlue_(a, b)
#define RaddbgTypeView(type, ...) \
__attribute__((section(".raddbg"), used)) static char RaddbgGlue(raddbg_data__, __COUNTER__)[] = \
("type_view: {type: ```" #type "```, expr: ```" #__VA_ARGS__ "```}")

RaddbgTypeView(String8,  array(data, size));
RaddbgTypeView(String16, array(data, size));
RaddbgTypeView(String32, array(data, size));

RaddbgTypeView(String8Node,  value);
RaddbgTypeView(String16Node, value);
RaddbgTypeView(String32Node, value);

RaddbgTypeView(String8List,  list(first, next));
RaddbgTypeView(String16List, list(first, next));
RaddbgTypeView(String32List, list(first, next));

RaddbgTypeView(TokenNodeList, list(root, next));
RaddbgTypeView(ModuleList,    list(root, next));

RaddbgTypeView(Arena,        rows($, current_position, capacity, previous, base_position));
RaddbgTypeView(ScratchArena, rows($, arena, start_position));

#endif

#endif // BASE_RADDBG_H
