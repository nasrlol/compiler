internal void
debug_str8_diff(TokenTree *tree, MemArena *arena, u64 diff_size, const char *function_name)
{
    ScratchArena scratch = scratch_start(arena);
    String8 debug_log = PushString8(arena, diff_size);
    debug_log.data = tree->buffer->data;
    debug_log.size = diff_size;
    Log("call: %s \t" Str8Fmt "\n", function_name, Str8Arg(debug_log));
    scratch_end(&scratch);
}

// create tokens, from a certain starting position until a certain
// delimiter. build the tokens and push them onto the tree
internal void
push_token_children(MemArena *arena, TokenTree *tree, String8List *delimiters)
{
    ScratchArena scratch = scratch_start(arena);
    String8 *temp_buffer = PushStruct(arena, String8);
    MemCpy(temp_buffer, tree->buffer, tree->buffer->size);
    
    FOR_STR8_NODE(delimiters, curr_delimiter) {
        FOR_STR8_MATCH(*temp_buffer, curr_delimiter->value) {
            TokenNode *node = PushStruct(arena, TokenNode);
            TokenNode *current = tree->current;
            
            // NOTE: we can assume that if the first node is nil that the last node will be nill aswell
            if(!is_token_node_nil(current->first)) {
                current->first = node;
                current->last  = node;
            } else {
                node->previous = current->last;
                node->parent   = current->parent;
                current->last->next = node;
                current->last = node;
            }
        }
    }
    
    scratch_end(&scratch);
}

internal Token
consume_token(String8 *buffer)
{
    u64 start_index = 0;
    u64 end_index   = 0;
    String8 substr = {};
    
    // iterate the buffer and find the whitespaces
    FOR_STR8_CHOP(index, buffer, 1, str8_chop_left_ptr) {
        b32 result = cstring_is_whitespace(*buffer->data);
        if(result && start_index == 0) {
            
            continue;
        } else if(start_index != 0) {
            
            end_index = index;
        }
        start_index = index;
        
        if(start_index != 0 && end_index != 0)
        {
            substr = str8_create_substr(buffer, index, start_index, end_index);
            
            // TODO: guess the token type;
            if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) {
            } else if(str8_is_match(*buffer, substr)) { }
            
            
        }
        
        
        return  {
            
            // just for reference for now
#if 0
            enum TokenType type;
            u64 line;
            
            union {
                String8 str_v;
                f64 f64_v; // this is a seperate type because floats have a different notation than integers do
                u64 int_v;
            } value;
            
#endif
        };
        
    }
    
    return nil_token; // we return an invalid token if we can't find a token  to consume
    
}

internal void
push_token_node(TokenTree *tree, TokenNode *node)
{
    if(is_token_node_nil(tree->root)) {
        tree->root    = node;
        tree->current = node;
        return;
    } else {
        tree->current->next = node;
        node->parent        = tree->current;
        
        // Fill the token node with nil nodes if they are not empty
        {
            FillTokenNodeFieldWithNilNode(next);
            FillTokenNodeFieldWithNilNode(previous);
            FillTokenNodeFieldWithNilNode(child);
            FillTokenNodeFieldWithNilNode(parent);
            FillTokenNodeFieldWithNilNode(first);
            FillTokenNodeFieldWithNilNode(last);
        }
    }
}

internal TokenTree *
tokenize_tree_fmt(MemArena *arena, String8 *buffer_from_file)
{
    // initializations
    b32 is_empty = str8_is_empty(buffer_from_file);
    Assert(!is_empty);
    TokenTree *tree = PushStruct(arena, TokenTree);
    tree->buffer = PushStruct(arena, String8);
    MemCpy(tree->buffer, buffer_from_file, buffer_from_file->size);
    tree->buffer->size = buffer_from_file->size;
    tree->index = 0;
    
    FOR_STR8_CHOP(index, *tree->buffer, 1, str8_chop_left) {
        ScratchArena cycle_scratch = scratch_start(arena);
        
        u8 current_character = tree->buffer->data[0];
        
        // handle skipping
        b32 is_whitespace = cstring_is_whitespace(current_character);
        if(is_whitespace) {
            continue;
        }
        
        b32 is_newline = cstring_is_newline(current_character);
        if(is_newline) {
            tree->amount_of_new_lines += 1;
            continue;
        }
        
        TokenNode *node = PushStruct(arena, TokenNode);
        Token *token = PushStruct(arena, Token);
        String8List *delimiters = PushStruct(arena, String8List);
        
        if(cstring_is_alpha(current_character)) {
            u64 index = 0;
            
            String8 *temp_buffer = PushStructZero(arena, String8);
            
            b32 is_alpha = cstring_is_alpha_num(current_character);
            FOR_STR8_ARB_FN(is_alpha, str8_chop_left, *temp_buffer, 1)
            {
                ++index;
            }
            
            
            // 8/2/2026 TODO: but we don"t initialize the root node at all?
            // we talk about the children but we ignore the root node
            // and what is the point of focussing on this here when we have a function that handles this?
            
            // the tree root only takes children. because when we take a look at the tree. the root having a next node means that something is on the same height
            // as that root node. this is only appropriate when working with translation units. i think we could have a forest of trees. where the root nodes/
            // stand for the translation units. and therefor allow the multiple notes to interact with each other.
            // initialize root node
            if(is_token_node_nil(tree->root))
            {
                tree->root->child = node;
            }
            ////////////////////////////////
            
            token->value.str_v = str8_chop_left(*tree->buffer, index);
            token->type = TokenType::IdentToken;
        }
        
        else if(cstring_is_digit(current_character)) {
            if(node->parent->token->type == TokenType::IdentToken) {
                node->parent->flag = (TokenNodeFlags)0;
                node->parent->token->type = TokenType::IdentToken;
            } else {
                node->parent->flag = TokenNodeFlags::Bad;
                break;
            }
        } else if(current_character ==  '\"') {
            token->type = TokenType::IdentToken;
        } else if(str8_any_match(*tree->buffer,str8("//"))) {
            u64 start_pos = tree->index;
            String8 *buffer = tree->buffer;
            u64 offset = cstring8_find_needle(tree->buffer->data, '\n');
            tree->index += offset;
            
            DEBUG_STR8_DIFF(tree, arena, offset);
            str8_chop_left_ptr(tree->buffer, offset);
            
        } else if(str8_any_match(*tree->buffer, str8("/*"))) {
            
            String8 element = str8("*/");
            u64 offset = str8_find(tree->buffer, 0, element);
            u64 total_offset = offset + element.size; // calculate in the size of the element. [ */ ]. the exact reason don't care to explain
            DEBUG_STR8_DIFF(tree, arena, total_offset);
            str8_chop_right_ptr(tree->buffer, offset);
            
        } else if(current_character == '{') {
            
            tree->current->next = node;
            node->token->type = (enum TokenType)'{';
            String8Node right_paran_delim = str8_node("}");
            str8_list_push_node(delimiters, &right_paran_delim);
            
        } else if(current_character ==  '}') {
            if(node->first->token->type != (enum TokenType)'{') {
                Log("Error: closing brace not matching opening brace");
            } else {
                Token *token = PushStruct(arena, Token);
                tree->current->last = node;
                
                token->type = TokenType::Param;
                token->line = tree->amount_of_new_lines;
                token->value.str_v = str8("}");
                
                
                if(is_token_node_nil(tree->current->last)) {
                    node->token = token;
                    node->flag = TokenNodeFlags::Child;
                    node->previous = tree->current;
                    node->parent = node->previous->parent;
                    
                    FillTokenNodeFieldWithNilNode(last);
                    FillTokenNodeFieldWithNilNode(first);
                    FillTokenNodeFieldWithNilNode(child);
                    FillTokenNodeFieldWithNilNode(next);
                } else {
                    TokenNode *temp_current_token_node = tree->current->last->next;
                    temp_current_token_node->token = token;
                    temp_current_token_node->flag =  TokenNodeFlags::Invalid;
                    temp_current_token_node->previous = tree->current->last;
                    temp_current_token_node->parent = tree->current->last->parent;
                }
            }
        } else if(current_character == '(') {
            if(tree->current->parent) {
                String8Node *right_paran = PushStruct(arena, String8Node);
                *right_paran = str8_node(")");
                str8_list_push_node(delimiters, right_paran);
                push_token_children(arena, tree, delimiters);
                push_token_node(tree, node);
            }
        } else if(current_character == ')') {
            // TODO: fix this ugly thing
            if(str8_is_match(tree->current->first->token->value.str_v, str8(")"))) {
                Log("Error: No opening brace found on line  %llu amount_of_newlines", tree->amount_of_new_lines);
            } else {
                // TODO: Something I forgot what
            }
        }
        // handle arrays --  an array consists of children of the variable name
        // so we just do a children until we hit the delimiter
        else if(current_character == '[') {
            String8List *delimiters = PushStruct(arena, String8List);
            String8Node *right_bracket = PushStruct(arena, String8Node);
            *right_bracket = str8_node("]");
            str8_list_push_node(delimiters, right_bracket);
            push_token_node(tree, node);
        } else if(current_character == ']') {
            if(tree->current->first->token->type != (enum TokenType)'['){
                Log("didn't close the bracket you moron");
            }
            
        } else if(current_character == '#') {
            Token pound_token = consume_token(tree->buffer);
            if(pound_token.value.str_v.size != 0) // TODO: can this be null and segfault if we don't initialize it?
            {
                
                create_module(arena, pound_token);
                
            } else {
                
                PANIC(str8("No directive found after pound sign"));
                
            }
            
            // TODO: pop the pound_token from the stack. we dont need it anymore after making the module
        } else if(current_character == ':') {
            if(tree->current->parent && tree->current->parent->token->type  == TokenType::IdentToken) {
                
                String8List *delimiters = PushStruct(arena, String8List);
                String8Node *colon_node = PushStruct(arena, String8Node);
                String8Node *komma_node = PushStruct(arena, String8Node);
                
                str8_list_push_node(delimiters, colon_node);
                str8_list_push_node(delimiters, komma_node);
                push_token_children(arena, tree, delimiters);
            } else {
                // TODO: push a warning / error
                Log("syntax error. Issue with the previous token. Unexpected token.");
            }
        } else {
            // TODO:
            PANIC(str8("Not valid syntax"));
        }
        
    }
    
    return tree;
}

