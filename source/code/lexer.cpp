//- nil variable functions
                           internal b32
    is_token_node_nil(TokenNode *node)
    {
    return (node == &nil_token_node);
    }

    internal b32
    is_token_nil(Token *token)
    {
    return (token == &nil_token);
    }

    //- token tree helper functions

    // NOTE: I'm thinking this should return a handle to the position after skipping the comments
    internal u64
    handle_c_style_comments(TokenTree *tree, u64 start_pos)
    {
    // TODO: Handle c style comment
    return 0;
    }

    // TODO: change the signature return a handle to where we skipped with the comments
    internal void
    handle_cpp_style_comments(String8 *buffer, u64 start_pos)
    {

    for(u64 idx = start_pos; buffer->data[idx] != '\n'; ++idx)
    {
    if(cstring_is_slash(buffer->data[idx]) && cstring_is_slash(buffer->data[idx + 1]))
    {
    str8_find(buffer, idx, str8("\n"));
    }
    }
    return;
    }

    // create tokens, from a certain starting position until a certain
    // delimiter. build the tokens and push them onto the tree


    internal void
push_token_children(MemArena *arena, TokenTree *tree, String8List *delimiters)
    {
ScratchArena scratch = scratch_start(arena);
    String8 *temp_buffer = PushStruct(arena, String8);
MemCpy(temp_buffer, tree->buffer, tree->buffer->size);

FOR_STR8_NODE(delimiters, curr_delimiter)
    {
    FOR_STR8_MATCH(*temp_buffer, curr_delimiter->value)
    {
    TokenNode *node = PushStruct(arena, TokenNode);
    TokenNode *current = tree->current;

    // NOTE: we can assume that if the first node is nil that the last node will be nill aswell
    if(!is_token_node_nil(current->first))
    {
    current->first = node;
    current->last  = node;
    }
    else
    {
    node->previous = current->last;
    node->parent   = current->parent;
    current->last->next = node;
    current->last = node;
    }
    }
    }

scratch_end(&scratch);
    }

    internal void
    push_token_node(TokenTree *tree, TokenNode *node)
    {
    if(is_token_node_nil(tree->root))
    {
    tree->root    = node;
    tree->current = node;
    return;
    }
    else
    {
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
    TokenTree *tree = PushStruct(arena, TokenTree);
    tree->buffer = buffer_from_file;

    if(tree->buffer->size == 0)
    {
    Log("Empty buffer");
    return NULL;
    }

    // initialize the root node
    tree->buffer = PushStruct(arena, String8);
    MemCpy(tree->buffer, buffer_from_file, sizeof(buffer_from_file));
    tree->index = 0;

    FOR_STR8_CHOP(tree->buffer, tree->buffer->size, str8_chop_left, 1)
    {
    ScratchArena cycle_scratch = scratch_start(arena);

    u8 current_character = tree->buffer->data[0];
    if(!cstring_is_whitespace(current_character) || !cstring_is_slash(current_character))
    {
    continue;
    }

    if(cstring_is_newline(current_character))
    {
    tree->amount_of_new_lines += 1;
    continue;
    }

    TokenNode *node = PushStruct(arena, TokenNode);
    Token *token = PushStruct(arena, Token);

    if(cstring_is_alpha(current_character))
    {
    u64 index = 0;

    String8 *temp_buffer = PushStructZero(arena, String8);

    b32 is_alpha = cstring_is_alpha_num(current_character);
    FOR_STR8_ARB_FN(is_alpha, str8_chop_left, *temp_buffer, 1)
    {
    ++index;
    }


    // 8/2/2026 TODO: but we don't initialize the root node at all?
    // we talk about the children but we ignore the root node
    // and what is the point of focussing on this here when we have a function that handles this?

    // the tree root only takes children. because when we take a look at the tree. the root having a next node means that something is on the same height
    // as that root node. this is only appropriate when working with translation units. i think we could have a forest of trees. where the root nodes/
    // stand for the translation units. and therefor allow the multiple notes to interact with each other.
    // initialize root node
    #if 0
    if(is_token_node_nil(tree->root)) tree->root->child = node;
    #endif
    ////////////////////////////////

    token->value.str_v = str8_chop_left(*tree->buffer, index);
    token->type = TokenType::Identifier;
    }

    else if(cstring_is_digit(current_character))
    {
    // TODO: Error unless the previous token was a declaration token and thus this needs to be handled
    // as a value intead of a declaration

    if(node->parent->token->type == TokenType::Identifier)
    {
    node->parent->flag = TokenNodeFlags::None;
    node->parent->token->type = TokenType::Identifier;
    }
    else
    {
    node->parent->flag = TokenNodeFlags::Bad;
    }
    }
    else if(str8_is_match(*tree->buffer, str8('"')))
    {
    // TODO: find the closing quote
    token->type = TokenType::Identifier;
    }
    else if(str8_is_match(*tree->buffer,  str8('\'')))
    {
    // TODO: start pos for handling cpp style comments index handling
    // change the function signature because the current position of the buffer.
    // is the positoin of the '/'. because we chop the str8
    #if 0
    handle_cpp_style_comments(tree->buffer, index);
    #endif
    }
    else if(str8_is_match(*tree->buffer, str8('{')))
    {
    String8 closed_param_delim = str8("}");

    // NOTE: an open brace defines a new scope
// TODO: push token children. using the new function signature
    tree->current->next = node;
    node->token->type    = (enum TokenType)'{';
    }
    else if(str8_is_match(*tree->buffer, str8('}')))
    {
    if(node->first->token->type != (enum TokenType)'{')
    {
    Log("Error: closing brace not matching opening brace");
    }
    else
    {
    Token *token = PushStruct(arena, Token);
    tree->current->last = node;

    // TODO: extract into function later
    {
    token->type = TokenType::Param;
    token->line = tree->amount_of_new_lines;
    token->value.str_v = str8("}");
    }

    if(is_token_node_nil(tree->current->last))
    {
    // TODO: Fill in the token
    node->token;
    node->flag = TokenNodeFlags::Child;
    node->previous = tree->current;
    node->parent   = node->previous->parent;

    FillTokenNodeFieldWithNilNode(last);
    FillTokenNodeFieldWithNilNode(first);
    FillTokenNodeFieldWithNilNode(child);
    FillTokenNodeFieldWithNilNode(next);
    }
    else
    {
    TokenNode *temp_current_token_node = tree->current->last->next;
    temp_current_token_node->token = token;
    temp_current_token_node->flag =  TokenNodeFlags::None;
    temp_current_token_node->previous = tree->current->last;
    temp_current_token_node->parent = tree->current->last->parent;
    }
    }
    }
    else if(str8_is_match(*tree->buffer, str8('(')))
    {
    if(tree->current->parent)
    {
String8List *delimiters = PushStruct(arena, String8List);
				String8Node node =
{
NULL,
str8(')'),
};

    str8_list_push_node(delimiters, &node);
					push_token_children(arena, tree,delimiters);
    }
    }
    else if(str8_is_match(*tree->buffer, str8(')')))
    {
// TODO: fix this ugly thing
    if(str8_is_match(tree->current->first->token->value.str_v, str8(')')))
    {
    Log("Error: No closing brace found on line  %llu amount_of_newlines", tree->amount_of_new_lines);
    }
    else
    {
    // TODO: Something I forgot what
    }
    }
    // handle arrays
    else if(str8_is_match(*tree->buffer, str8("[")))
    {
    // an array consists of children of the variable name
    // so we just do a children until we hit the delimiter

String8List *delimiters = PushStruct(arena, String8List);
String8Node node =
{
NULL,
str8("]"),
};

str8_list_push_node(delimiters, &node);
    push_token_children(arena, tree, delimiters);
    }
    else if(str8_is_match(*tree->buffer, str8("]")))
    {
    if(tree->current->first->token->type != (enum TokenType)'[')
{
    Log("didn't close the bracket you moron");
    }
}
    else if(str8_is_match(*tree->buffer, str8("#")))
    {
    // TODO HANDLE MODULE
    }
    else if(str8_is_match(*tree->buffer, str8(':')))
    {
    if(tree->current->parent->token->type  == TokenType::Identifier)
    {

    String8List *sub_delimiters = PushStruct(arena, String8List);
    String8Node *sub_colon_node = PushStruct(arena, String8Node);
    String8Node *sub_komma_node = PushStruct(arena, String8Node);


    str8_list_push_node(sub_delimiters, sub_colon_node);
    str8_list_push_node(sub_delimiters, sub_komma_node);

// TODO: use the new function signature for push_token_children
#if 0
    push_token_children(tree, delimiter, sub_delimiters);
#endif
    }
    }
    else
    {
    Log("Bad token");

    }
    }

    return tree;
}
