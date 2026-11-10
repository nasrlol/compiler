// TODO: replace the logs with this
internal void debug_str8_diff(TokenTree *tree, Arena *arena, u64 diff_size,
			      const char *function_name, String8 buffer)
{
    ScratchArena scratch   = scratch_start(arena);
    String8      debug_log = PushString8(arena, diff_size);
    Unused(debug_log);
    MemCpy(&debug_log, &buffer, buffer.size);
    debug_log.size = diff_size;
    Log("call: %s \t" Str8Fmt, function_name, Str8Arg(debug_log));
    scratch_end(&scratch);
}

// create tokens, from a certain starting position until a certain
// delimiter. build the tokens and push them onto the tree
internal void push_token_children(Arena *arena, String8 buffer, TokenTree *tree, String8List *delimiters)
{
    String8 buffer_copy = PushString8(arena, buffer.size);
    MemCpy(&buffer_copy, &buffer, buffer.size);
	
    FOR_STR8_NODE(delimiters, delimiter_node) {
        FOR_STR8_MATCH(buffer_copy, delimiter_node->value) {
            TokenNode *child_node    = PushStruct(arena, TokenNode);
            TokenNode *current_node = tree->current;
			
            // NOTE: we can assume that if the first node is nil, the last node will be nil
            // as well
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

internal void push_token_node(TokenTree *tree, TokenNode *node)
{
    if(is_token_node_nil(tree->root) || is_token_node_nil(tree->current)) {
        tree->root    = node;
    }
	
    tree->current->previous = tree->current;
    tree->current->next     = node;
    node->parent            = tree->current;
    tree->current           = node;
	
    FillTokenNodeFieldWithNilNode(next);
    FillTokenNodeFieldWithNilNode(previous);
    FillTokenNodeFieldWithNilNode(child);
    FillTokenNodeFieldWithNilNode(parent);
    FillTokenNodeFieldWithNilNode(first);
    FillTokenNodeFieldWithNilNode(last);
}

internal TokenTree *parse_expression(Arena *arena, String8 expression)
{
    TokenTree *epxression_tree = PushStructZero(arena, expression_tree);
    ScratchArena scratch = scratch_start(arena);
    scratch_end(&scratch);




}

internal TokenTree *tokenize_tree(Arena *arena, String8 buffer)
{
    AssertLog(!str8_is_empty(&buffer), "Buffer is empty");
    TokenTree *tree = PushStructZero(arena, TokenTree);
	
    ScratchArena iteration_scratch = {};
    String8 iteration_buffer = PushString8(arena, buffer.size);
	
    for(u64 buffer_index = 0; buffer_index < iteration_buffer.size; ++buffer_index) {
		
	iteration_scratch = scratch_start(arena);
	MemCpy(&iteration_buffer, &buffer, buffer.size);
        u8 current_character = iteration_buffer.data[buffer_index];
		
        b32 is_whitespace = cstring_is_whitespace(current_character);
        if(is_whitespace) {continue;}
		
	// NOTE: do one extra iteration to skip the newline. we keep getting  stuck on the current character otherwise
        b32 is_newline = cstring_is_newline(current_character);
        if(is_newline) {
            tree->nl_count += 1;
            iteration_buffer = str8_chop_left(iteration_buffer, 1);
            continue;
        }
		
        TokenNode *node = PushStructZero(arena, TokenNode);
	node->token = PushStructZero(arena, Token);
	node->expected_delimiters = PushStructZero(arena, String8List);
		
	if(cstring_is_digit(current_character)) {

            if(node->parent->token->type == TokenType::IdentToken) {
                node->parent->flag        = (TokenNodeFlags)0;
                node->parent->token->type = TokenType::IdentToken;
            } else {
                node->parent->flag = TokenNodeFlags::Bad;
                break;
            }

        } else if(current_character == '{') {
            tree->current->next           = node;
            node->token->type             = (enum TokenType)'{';

	    // TODO: how do we notify the global state that we are looking for a closing brace?
	    {
		String8Node right_brace_node = str8_node("}");
		str8_list_push_node(delimiters, &right_brace_node);
	    }
			
        } else if(current_character == '\"') {
            // token->type == TokenType:IdentToken;
			
        } else if(current_character == '}') {
            if(node->first->token->type != (enum TokenType)'{') {
                Log("Error: '}' does not match an opening '{'");
            } else {
                Token *brace_token  = PushStruct(arena, Token);
                tree->current->last = node;
                brace_token->type         = TokenType::Param;
                brace_token->line         = tree->nl_count;
                brace_token->as_str = str8("}");
				
                if(is_token_node_nil(tree->current->last)) {
                    node->token    = brace_token;
                    node->flag     = TokenNodeFlags::Child;
                    node->previous = tree->current;
                    node->parent   = node->previous->parent;
					
                    FillTokenNodeFieldWithNilNode(last);
                    FillTokenNodeFieldWithNilNode(first);
                    FillTokenNodeFieldWithNilNode(child); FillTokenNodeFieldWithNilNode(next); } else { TokenNode *next_node = tree->current->last->next;
                    next_node->token     = brace_token;
                    next_node->flag      = TokenNodeFlags::Invalid;
                    next_node->previous  = tree->current->last;
                    next_node->parent    = tree->current->last->parent;
                }
            }
        } else if(current_character == '(') {
            if(tree->current->parent) {
                String8Node *right_paren_node = PushStruct(arena, String8Node);
                *right_paren_node             = str8_node(")");
                str8_list_push_node(delimiters, right_paren_node);
                push_token_children(arena, iteration_buffer, tree, delimiters);
                push_token_node(tree, node);
            }
        } else if(current_character == ')') {
            // TODO: fix this ugly thing
            if(str8_is_match(tree->current->first->token->as_str, str8(")"))) {
                push_token_node(tree, node);
            } else {
                Log("Error: ')' on line %llu has no matching '('", tree->nl_count);
            }
        }
        // NOTE: handle arrays --  an array consists of children of the variable name
        // so we just do a children until we hit the delimiter
        else if(current_character == '[') {
            String8List *bracket_delimiters    = PushStruct(arena, String8List);
            String8Node *right_bracket_node = PushStruct(arena, String8Node);
            *right_bracket_node             = str8_node("]");
            str8_list_push_node(bracket_delimiters, right_bracket_node);
            push_token_node(tree, node);
        } else if(current_character == ']') {
            if(tree->current->first->token->type != (enum TokenType)'[') {
                Log("Error: ']' has no matching '['");
            }
        } else if(current_character == ':') {
            if(tree->current->parent &&
               tree->current->parent->token->type == TokenType::IdentToken) {
                String8List *field_delimiters = PushStruct(arena, String8List);
                String8Node *colon_node = PushStruct(arena, String8Node);
                String8Node *comma_node = PushStruct(arena, String8Node);
				
                str8_list_push_node(field_delimiters, colon_node);
                str8_list_push_node(field_delimiters, comma_node);
                push_token_children(arena, iteration_buffer, tree, field_delimiters);
            } else {
                // TODO: push a warning / error
                Log("Error: unexpected ':', expected an identifier before it");
            }
			
	} else if(cstring_is_alpha(current_character)) {
			
			
	    u64 index = 0;
	    if(is_token_node_nil(tree->root)) {
		tree->root->child = node;
	    }
			
	    if(token) {
		token->as_str = str8_chop_left(iteration_buffer,  buffer_index);
		token->type         = TokenType::IdentToken;
		node->token = token;
	    }
			
			
	    if(str8_starts_with(buffer, str8("struct"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("enum"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("if"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("else"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("for"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("in"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("while"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("switch"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("case"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("return"))) {
		push_token_node(tree, node);
	    } else if(str8_starts_with(buffer, str8("defer"))) {
		push_token_node(tree, node);
	    }
			
	}
    }
	
    return tree;
}

