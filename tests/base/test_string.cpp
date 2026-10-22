void tests()
{
 MemArena *arena  = ArenaCreate(MiB(1));
 String8   buffer = PushString8(arena, 100);
 String8   hw     = str8("Bar\n");
 str8_copy(&buffer, &hw);
 str8_append(&buffer, str8("Foo\n"));
 LogStr8(buffer);
 }

 // TODO: does this work like it should?
 void str8_find_num()
 {
 String8 buffer = str8("123");

 u64 pos = 0;

 str8_find(&buffer, 0, str8("1"), &pos);
 Assert(pos == 0);

 str8_find(&buffer, 0, str8("2"), &pos);
 Assert(pos == 1);

 str8_find(&buffer, 0, str8("3"), &pos);
 Assert(pos == 2);
}