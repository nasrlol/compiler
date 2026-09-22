// TODO: handle directives like @import etc

internal Token *
consume_token_from_buffer(MemArena *arena, String8 *buffer)
{
Assert(buffer->size < 0);
// TODO: Break out of function if no pound sign is found
  u64 offset_to_first_pound = 0;
b32 found = str8_find(buffer, 0, str8("#"), &offset_to_first_pound);
if(!found) return &nil_token;
str8_chop_left_ptr(buffer,offset_to_first_pound );

u8 pound_sign = (u8)'#';
if(buffer->data[0] == pound_sign) { // skip pound token
buffer->data++;
} else {
Assert(1);
}

//- NOTE: find the start_idx.
// iterate the buffer and find the whitespaces
// FIX: WRONG the pound sign should never be followed by a space
// NOTE: I don't know. I know casey prefers using for loops for everything but arent while loops easier to debug?
u64 idx = 0;
b32 is_whitespace  = 0;
u64 end_offset = 0;
String8 substr = {};

while(!is_whitespace) {
is_whitespace = cstring_is_whitespace(buffer->data[idx]);
end_offset = idx;
++idx;
}

if(end_offset != 0) {
substr = str8_create_substr(buffer, 0, end_offset);
str8_chop_left_ptr(buffer,  end_offset);
}

//- handle directives

if(str8_is_match(substr, str8("import"))) {

str8_chop_left_ptr(buffer, substr.size);
while(cstring_is_whitespace(*buffer->data)) str8_chop_left_ptr(buffer, 1);
u64 whitespace_pos = 0;
str8_find(&substr, 0, str8(" "), &whitespace_pos);
 substr = str8_create_substr(buffer, 0, whitespace_pos);


} else {
		if(str8_is_empty(&substr))  {
			PANIC("directive not defined yet");
			return &nil_token;
		}
PANIC("Invalid import");
}


// NOTE: before there was a pice of code here that said that we should handle module code the code here
// but this should stay a generic function.
// do the matrching on the returned struct instead

Token *new_token = PushStruct(arena, Token);

new_token->type = IdentToken;
new_token->line = 0;
new_token->value.as_str = substr;

new_token->type = IdentToken;
new_token->line = 0;
new_token->value.as_str = substr;

return new_token;

// NOTE: We return an invalid token if we can't find a one to consume.
// see above.

}


// TODO: I will derefernce this for now. but think about it if you really want it to derefernce it?
// first initial pass to handle the includes and create on single large buffer
// but is this the approach we want?
//- rewrite the buffer to handle module imports
internal void
scan_buffer_and_handle_modules(MemArena *arena ,String8 *original_buffer) {

String8 dest_buffer = PushString8(arena, original_buffer->size);
str8_copy(&dest_buffer, original_buffer);

Token *token = consume_token_from_buffer(arena, original_buffer);
String8List *files;
if(!is_token_nil(token)){
 files = DirLoad(arena, &token->value.as_str);
} else {
// NOTE: if it's a nil token we should be able to assume that there is no pound token in the file
// or that the directive used isnt known. or some other edge case.
// this will be logged from inside the consume_token_from_buffer function
Log("preprocessor early exit");
return;
}

String8 buffer;

FOR_STR8_NODE(files, current) {
if(str8_is_match(current->value, token->value.as_str)) {

String8 current_value_file_path = current->value;

if(!buffer.size) {
buffer = FileLoad(arena, current_value_file_path);
} else {
str8_append(&buffer, FileLoad(arena, current_value_file_path));
}
buffer = FileLoad(arena, current->value);
}
}

Log("preprocessor exit");
return;


// TODO: ...
}