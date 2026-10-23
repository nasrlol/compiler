internal String8 handle_all_comments(String8 buffer) {
		b32 found = false;
		while(!found) {
			u64 comment_position = 0;
			found = str8_find(buffer, 0, str8("//"), &comment_position);
			{
				// TODO: I dont know if this check works?
				// TODO: What if we are at the beginning of the file.
				// if(start_pos != 0) { // TODO: this logic is inverted i think
				// if(buffer->data[start_pos - 1] == '\n') { // is the current line the start of the line
				
				u64 start_pos = 0;
				
				if(false) {
					Log("not the beginning of the line");
				} else {
					u64 new_line = 0;
					str8_find(buffer, start_pos, str8("\n"), &new_line);
					str8_chop_left_ptr(buffer,  new_line);
				}
			}
		}
	}
	

// What does this function do again?
internal String8 handle_directives(MemArena *arena, String8 buffer)
{
 Assert(buffer.size);
 TokenNodeList *list_pound_token_values = PushStruct(arena, TokenNodeList);
 u64 offset_to_first_pound              = 0;
 b32 found                              = str8_find(buffer, 0, str8("#"), &offset_to_first_pound);

// if no pound sign was found in the entire buffer we can just leave
 if(!found){ return buffer; }

 // NOTE: do +1 to remove the pound sign and enable us to start parsing the directive
 str8_chop_left_ptr(buffer, offset_to_first_pound + 1);


 //-
 u64 idx           = 0;
 b32 is_whitespace = 0;
 u64 end_offset    = 0;
 String8 substr    = {};

 while (!is_whitespace && idx < buffer->size) {
  is_whitespace = cstring_is_whitespace(buffer->data[idx]);
  end_offset    = idx;
  ++idx;
 }

 if (end_offset != 0) {
  substr = str8_create_substr(buffer, 0, end_offset);
  str8_chop_left_ptr(buffer, end_offset);
 }

 //- handle directives

 if (str8_is_match(substr, str8("import"))) {

  substr = str8_chop_left(*buffer, substr.size);

  while (cstring_is_whitespace(*buffer->data)) {
   str8_chop_left_ptr(buffer, 1);
  }

  u64 end_pos = 0;
  b32 found_first_newline_or_whitespace = false;
  for(u64 substr_index_to_find_newline_or_whitespace = 0; substr_index_to_find_newline_or_whitespace < substr.size; ++substr_index_to_find_newline_or_whitespace) {

   if(cstring_is_whitespace(buffer->data[substr_index_to_find_newline_or_whitespace]) ||
      cstring_is_newline(buffer->data[substr_index_to_find_newline_or_whitespace])) {
    end_pos = substr_index_to_find_newline_or_whitespace;
    found_first_newline_or_whitespace = true;
    continue;
   }

   if(found_first_newline_or_whitespace) {
    break;
   }

  }

  // TODO: we count one to many for some reason
  end_pos-= 1;

  substr = str8_create_substr(buffer, 0, end_pos);

 } else {
  if (str8_is_empty(&substr)) {

   Log("failed substring: " Str8Fmt, Str8Arg(substr));
   PANIC("directive not defined yet");
   return &nil_token;
  }
  PANIC("Invalid import");
 }

 // NOTE: before there was a pice of code here that said that we should handle module code the
 // code here but this should stay a generic function. do the matrching on the returned struct
 // instead

 Token *new_token = PushStruct(arena, Token);
 new_token->type         = IdentToken;
 new_token->line         = 0;
 new_token->value.as_str = substr;
 return new_token;
}
