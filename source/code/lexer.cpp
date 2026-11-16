internal b32 is_token_node_nil(TokenNode *node)
{
	
	return node == NULL || node == &nil_token_node;
}

internal void lexer_debug_str8_diff(TokenList *list, Arena *arena, u64 diff_size,
									const char *function_name, String8 buffer)
{
    ScratchArena scratch = scratch_start(arena);
    String8 debug_log = PushString8(arena, diff_size);
    Unused(debug_log);
    MemCpy(&debug_log, &buffer, buffer.size);
    debug_log.size = diff_size;
    Log("call: %s \t" Str8Fmt, function_name, Str8Arg(debug_log));
    scratch_end(&scratch);
}
#if 0
// create tokens, from a certain starting position until a certain
// delimiter. build the tokens and push them onto the list
internal void lexer_push_token_children(Arena *arena, String8 buffer, TokenList *list, String8List *delimiters)
{
    String8 buffer_copy = PushString8(arena, buffer.size);
    MemCpy(&buffer_copy, &buffer, buffer.size);
	
    FOR_STR8_NODE(delimiters, delimiter_node) {
        FOR_STR8_MATCH(buffer_copy, delimiter_node->value) {
            TokenNode *child_node    = PushStruct(arena, TokenNode);
            TokenNode *current_node = list->current;
			
            if(!is_token_node_nil(current_node->first)) {
                current_node->first = child_node;
                current_node->last  = child_node;
            } else {
                child_node->previous      = current_node->last;
                child_node->parent        = current_node->parent;
                current_node->last->next = child_node;
                current_node->last       = child_node;
            }
        }
    }
}
#endif

internal void lexer_push_token_node_token_list(TokenList *list, TokenNode *node)
{
    FillTokenNodeFieldWithNilNode(next);
	node->previous              = list->last;
	
    if(is_token_node_nil(list->first) || is_token_node_nil(list->last))
	{
        list->first             = node;
    }
	else
	{
		list->last->next        = node;
	}
	
	list->last                  = node;
	list->current               = node;
	list->size                  += 1;
}


internal b32 lexer_character_is_delimiter(u8 character)
{
	return character == '(' || character == ')' || character == '[' || character == ']' || character == '{' || character == '}' || character == '|' || character == ',' || character == '!' || character == '<' || character == '=' || character == '>' || character == '-' || character == '+' || character == '%' || character == '&' || character == '*' || character == '/' || character == ';';
}



internal String8 lexer_parse_expression(Arena *arena, String8 buffer)
{
	if(buffer.size == 0) return str8_zero();
	u8 current_character = buffer.data[0];
	
	if(current_character == '"')
	{
		u64 close_delimiter_index = 1;
		while(close_delimiter_index < buffer.size && buffer.data[close_delimiter_index] != '"')
		{
			// TODO: how do you handle an escape character
			close_delimiter_index += 1;
		}
        AssertLog(close_delimiter_index < buffer.size, "Error: unterminated string");
		return str8_create_substr(&buffer, 0, close_delimiter_index + 1);
		
	}
	
	if(lexer_character_is_delimiter(current_character))
	{
		return String8{buffer.data, 1};
	}
	
	if(cstring_is_alpha(current_character) || cstring_is_digit(current_character))
	{
		u64 length = 0;
		while(length < buffer.size &&
			  (cstring_is_alpha(buffer.data[length]) ||
			   cstring_is_digit(buffer.data[length]) ||
			   buffer.data[length] == '_')) {
			length += 1;
		}
		return str8_create_substr(&buffer, 0, length);
	}
	
	return String8{};
}


internal Keyword lexer_lexeme_is_keyword(String8 lexeme)
{
	for(u64 index = Keyword_None + 1; index < ArrayCount(reserved_words); ++index)
	{
		if(str8_is_match(lexeme, reserved_words[index]))
		{
			return (Keyword)index;
		}
	}
	return Keyword_None;
}


internal TokenList *lexer_tokenize(Arena *arena, String8 buffer)
{
	Assert(buffer.size);
	Assert(!str8_is_empty(&buffer));
	
	TokenList *list = PushStructZero(arena, TokenList);
	String8 local_buffer = PushString8(arena, buffer.size);
	BufferProperties p = {};
	
	str8_copy(&local_buffer, &buffer);
	
	
	while(local_buffer.size != 0)
	{
		
		u8 current_character = local_buffer.data[0];
		
		b32 is_newline       = cstring_is_newline(current_character);
		b32 is_whitespace    = cstring_is_whitespace(current_character);
		if(is_newline || is_whitespace)
		{
			if(is_newline)
			{
				p.newlines += 1;
			}
			local_buffer = str8_chop_left(local_buffer, 1);
			continue;
		}
		
		String8 lexeme = lexer_parse_expression(arena, local_buffer);
		// make token node
		{
			TokenNode *node = PushStructZero(arena, TokenNode);
			
			node->token = PushStructZero(arena, Token);
			
			// make token
			{
				Keyword k = lexer_lexeme_is_keyword(lexeme);
				// NOTE: Fixed,  eventhough its not a keyword its always an identifier of some sort
				{
					// node->token->keyword = k;
					node->token->value   = lexeme;
					lexer_push_token_node_token_list(list, node);
				}
				
			}
		}
		
		u64 chop_size = 0;
		if(lexeme.size == 0) chop_size = 1;
		else chop_size = lexeme.size;
		local_buffer = str8_chop_left(local_buffer, chop_size);
		
	}
	
	return list;
}


