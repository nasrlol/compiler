internal module
create_module(MemArena *arena, Token *token)
{

return {};
}


// TODO: I will derefernce this for now. but think about it if you really want it to derefernce it?
// first initial pass to handle the includes and create on single large buffer
// but is this the approach we want?
//- rewrite the buffer to handle module imports
internal void
scan_buffer_handle_modules(MemArena *arena, String8 *buffer) {


ScratchArena scratch = scratch_start(arena);
if(buffer->data[0] == '#') {

} else {
u64 pos = str8_find(buffer, 0, str8("#"));
if(pos == 0) return;  // no pound signs found
 u8 index = 0;
while (cstring_is_whitespace(buffer->data[index])) ++index;

}

Token *token = consume_token_from_buffer(arena, buffer);
  String8List *files = DirLoad(arena, &token->value.str_v);

scratch_end(&scratch);

    return;


// TODO: ...
}