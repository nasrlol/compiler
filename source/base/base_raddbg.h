#ifndef BASE_RADDBG_H
#define BASE_RADDBG_H

// RAD Debugger type views, embedded in the executable's ".raddbg" section.
// raddbg reads this section when it loads the binary, so the views can't be
// lost when raddbg rewrites project.raddbg.
// Only compiled in debug builds (build.sh passes -DDEBUG=1).

#if defined(DEBUG) && DEBUG && (defined(__GNUC__) || defined(__clang__))

#define RaddbgGlue_(a, b) a##b
#define RaddbgGlue(a, b)  RaddbgGlue_(a, b)
#define RaddbgTypeView(type, ...) \
__attribute__((section(".raddbg"), used)) static char RaddbgGlue(raddbg_data__, __COUNTER__)[] = \
("type_view: {type: ```" #type "```, expr: ```" #__VA_ARGS__ "```}")

//- strings: show the bytes as text
RaddbgTypeView(String8,  array(data, size));
RaddbgTypeView(String16, array(data, size));
RaddbgTypeView(String32, array(data, size));

//- string list nodes show their value, so a list expands to a column of strings
RaddbgTypeView(String8Node,  value);
RaddbgTypeView(String16Node, value);
RaddbgTypeView(String32Node, value);

//- singly linked lists: walk first -> next
RaddbgTypeView(String8List,  list(first, next));
RaddbgTypeView(String16List, list(first, next));
RaddbgTypeView(String32List, list(first, next));

//- compiler lists (they end in self-pointing nil sentinels; the list lens stops on the cycle)
RaddbgTypeView(TokenNodeList, list(root, next));
RaddbgTypeView(ModuleList,    list(root, next));

//- arenas: keep usage + chain, hide bookkeeping fields
RaddbgTypeView(Arena,        rows($, current_position, capacity, previous, base_position));
RaddbgTypeView(ScratchArena, rows($, arena, start_position));

#endif

#endif // BASE_RADDBG_H
