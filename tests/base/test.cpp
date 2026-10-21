#include "base/base_include.h"
#include "test_string.cpp"

typedef struct Foo Foo;
struct Foo {
    u8  bar;
    u64 baz;
};

extern void test_nil_checks();


int main()
{
    Log("\n\n\n>> Before resizing\n");
    MemArena *arena = ArenaCreate(MiB(100));
    Assert(arena);
    LogArena(arena);

#if 0
    Foo *foo = PushStruct(arena, Foo);;
    Foo *bar = PushStruct(arena, Foo);;
    Foo *baz = PushStruct(arena, Foo);;
#endif


    arena = ArenaResize(arena, arena->base_position, MiB(1), arena->capacity);

    Log("\n\n\n>> After resizing \n");
    LogArena(arena);


    str8_find_num();
}
