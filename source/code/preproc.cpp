internal String8 delete_all_matching_sections(Arena *arena, String8 buffer, String8 start, String8 end) {
	u64 index = 0;
	u64 start_pos = 0;
	while(str8_find(&buffer, index, start, &start_pos)) {
		
		u64 end_pos = 0;
		// TODO: if the nl position == size thats EOF or something
		str8_find(&buffer, index, end, &end_pos);
		
		u64 remove_end = end_pos + end.size;
		u64 tail       = buffer.size - remove_end;
		
		
		// NOTE: we get the comment posotion but it points to the first  character.
		// if the  start is bigger than 1. which when handling comments is
		// we substract 1 from the size. because we already have the first one and then add the rest
		// to  the comment position. which makes it get removed in the memmove
		MemMove((buffer.data + start_pos), (buffer.data + remove_end), (tail));
		buffer.size -= (end_pos - start_pos);
		index += start_pos;
	}
	return buffer;
}

internal String8 handle_comments(Arena *arena, String8 buffer) {
	buffer = delete_all_matching_sections(arena,  buffer,  str8("//"),  str8("\n"));
	buffer = delete_all_matching_sections(arena,  buffer, str8("/*"),  str8("*/"));
	return buffer;
}


// NOTE: we return a list a of  all pound signs that we found  within the current bufferr
// when find any 1..n we return the next tokens in a String8List.
internal String8List *handle_directives(Arena *arena, String8 buffer, String8 seperator)
{
	Assert(buffer.size);
	String8List *list = PushStruct(arena, String8List);
	TokenNodeList *list_pound_token_values = PushStruct(arena, TokenNodeList);
	u64 offset_to_first_pound              = 0;
	b32 found                              = str8_find(&buffer, 0, seperator, &offset_to_first_pound);
	if(!found){ return list; }
	// NOTE: do +1 to remove the pound sign and enable us to start parsing the directive
	buffer= str8_chop_left(buffer, offset_to_first_pound + 1);
	
	//-
	u64 idx           = 0;
	b32 is_whitespace = 0;
	u64 end_offset    = 0;
	String8 substr    = {};
	
	while (!is_whitespace && idx < buffer.size) {
		is_whitespace = cstring_is_whitespace(buffer.data[idx]);
		end_offset    = idx;
		++idx;
	}
	
	if(end_offset != 0) {
		substr = str8_create_substr(&buffer, 0, end_offset);
		buffer = str8_chop_left(buffer, end_offset);
	}
	
	//- handle directives
	
	if(str8_is_match(substr, str8("import"))) {
		
		substr = str8_chop_left(buffer, substr.size);
		while (cstring_is_whitespace(*buffer.data)) { buffer = str8_chop_left(buffer, 1); }
		
		u64 end_pos = 0;
		b32 found_first_newline_or_whitespace = false;
		for(u64 substr_index_to_find_newline_or_whitespace = 0; substr_index_to_find_newline_or_whitespace < substr.size; ++substr_index_to_find_newline_or_whitespace) {
			
			if(cstring_is_whitespace(buffer.data[substr_index_to_find_newline_or_whitespace]) ||
			   cstring_is_newline(buffer.data[substr_index_to_find_newline_or_whitespace])) {
				end_pos = substr_index_to_find_newline_or_whitespace;
				found_first_newline_or_whitespace = true;
				continue;
			}
			
			if(found_first_newline_or_whitespace) {
				break;
			}
			
		}
		
		end_pos-= 1;
		substr = str8_create_substr(&buffer, 0, end_pos);
		
		// check if the substr is an import cycle
		
		
	} else {
		if(str8_is_empty(&substr)) {
			
			Log("failed substring: " Str8Fmt, Str8Arg(substr));
			PANIC("directive not defined yet");
			
			// TODO: exit
			
		}
		
		PANIC("Invalid import");
	}
	
	// NOTE: before there was a pice of code here that said that we should handle module code the
	// code here but this should stay a generic function. do the matrching on the returned struct
	// instead
	
	Token *new_token = PushStruct(arena, Token);
	new_token->type         = IdentToken;
	new_token->line         = 0;
	new_token->as_str = substr;
	
	return list;
}

