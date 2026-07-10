#ifndef LEXER_H
#define LEXER_H

//-

// TODO: find a fix for this continuing casting to enum
enum struct TokenType
{
    // first 255 characters are reserved for ascii stuff
    Undefined = 256,
    Identifier, // if this is the case. we fall through to use the TokenFlags which do a beter job at specifying the properties of the identifier

    Value, // because an identfier could really be anything from a value, variable or function.

    String,
    Number,

    Double_Equal,
    Greater_Equal,
    Lesser_Equal,
    Param,

    Right_Shift,
    Left_Shift,
    Asterisk,

    // TODO: implement the token type setting for these
    Cpp_Style_Comment_Start,
    C_Style_Comment_Start,
    C_Style_Comment_End,
    };

    enum TokenNodeFlags
    {
    None  = (1 << 0),
    Root  = (1 << 11),
    Child = (1 << 12),
    Eof   = (1 << 13),
    Bad   = (1 << 14),
    };

    typedef struct Token Token;
    struct Token
    {
    enum TokenType type;
    u64            line; // good for line errors...
    u64            byte_offset;

    union
    {
    String8 string_value;
    f64     float64_value; // this is a seperate type because floats have a different notation than integers do
    u64     integer_value;
    } value;
    };

    typedef struct TokenNode TokenNode;
    struct TokenNode
    {
    TokenNodeFlags flag;

    #if 0
    union {
    TokenNode* reference_to_root_node; // case: we are not the root node so we reference to it to find out in which file we are
    String8 file_name;                 // case: we are the root node and we just store the name?
    // TODO:  of the file? the file or the path?
    } file_reference;
    #endif

    TokenNode *next;
    TokenNode *previous;

    TokenNode *child;
    TokenNode *parent;

    TokenNode *first;
    TokenNode *last;

    Token token; // do we want this as a pointer?
    };

    // find a way to do this without allocating the tokens on the heap. It's a huge toll on the performance of the program
    typedef struct TokenTree
    {
    TokenNode *root;
    TokenNode *current;

    String8 *buffer; // TODO: don't allocate this. it's a fixed sized buffer. but what we still take the reference of it instead of passing a copy?
    u8      *buffer_position;
    u64      index;
    } TokenTree;

    //- nil variables
    ReadOnly global_variable Token
    nil_token = {TokenType::Undefined, 0, 0, {NULL, 0}};

    #if 0
    ReadOnly global_variable TokenNode
    nil_token_node = {
    TokenNodeFlags::None, // TokenNodeFlags
    {NULL},    // nill reference for *reference_to_root_node

    NULL,      // next
    NULL,      // previous

    NULL,      // child
    NULL,      // parent

    NULL,      // first
    NULL,      // last

    {}         // token

    };
    #endif

    #if 1
    ReadOnly global_variable TokenNode
    nil_token_node = {

    TokenNodeFlags::None,
    #if 0
    & nil_token_node,        // file_reference
    #endif
    &nil_token_node, // next
    &nil_token_node, // previous
    &nil_token_node, // child
    &nil_token_node, // parent
    &nil_token_node, // first
    &nil_token_node, // last
    {}};
    #endif

    // TODO: removek this
    internal void
    fill_in_nil_token_node(TokenNode *nil_node)
    {
    #if 0
    nil_node->next = &nil_token_node;          // next
    nil_node->previous = &nil_token_node;      // previous
    nil_node->child = &nil_token_node;         // child
    nil_node->parent = &nil_token_node;        // parent
    nil_node->first = &nil_token_node;         // first
    nil_node->last = &nil_token_node;          // last
    nil_node->token = {};                      // token,
    #endif
    }

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

    #define FillTokenNodeFieldWithNilNode(field) \
    if(node->field == NULL || !is_token_node_nil(node)) \
    node->field = &nil_token_node;

    #if 0
    internal void
    fill_token_node_with_nil_nodes(TokenNode* node) {
    if (node->next == NULL)     node->next = &nil_token_node;
    if (node->previous == NULL) node->previous = &nil_token_node;
    if (node->child == NULL)    node->child = &nil_token_node;
    if (node->parent == NULL)   node->parent = &nil_token_node;
    if (node->first == NULL)    node->first = &nil_token_node;
    if (node->last == NULL)     node->last = &nil_token_node;
    }
    #endif

    // returns the index of up to where we skipped the comment
    internal u64
    handle_c_style_comments(TokenTree *tree, u64 start_pos)
    {
    u64 idx    = start_pos;
    u8 *buffer = tree->buffer->data;

    for(; !cstring_is_whitespace(buffer[idx]) || cstring_is_slash(buffer[idx]); ++idx)
    {
    }

    return idx;
    }

    internal void
    handle_cpp_style_comments(String8 *buffer, u64 start_pos)
    {

    for(u64 idx = start_pos; buffer->data[idx] != '\n'; ++idx)
    {
    if(cstring_is_slash(buffer->data[idx]) && cstring_is_slash(buffer->data[idx + 1]))
    {
    cstring_find_element(buffer->data, idx, (u8)"\n");
    }
    }
    return;
    }

    // !! TODO: refactor this overloaded function
    // create tokens and push them onto the tree
    internal void
    push_token_children(TokenTree *tree, u64 index, String8List delimiters)
    {
    b32 found_end = false;
    u8 *buffer    = tree->buffer->data;
    while(!found_end)
    {
    for(u64 buffer_idx = index; cstring_is_match(&buffer[buffer_idx], &delimiters); ++buffer_idx)
    {
    TokenNode *node = {0};
    // consume the token
    // consume while the buffer[index] is not a white space or delimiinter i guess
    for(u8 sub_delim_idx = 0; sub_delim_idx < delimiters->size; ++sub_delim_idx)
    {
    if(cstring_is_match(&sub_delim_idx->data[delimiters], &delimiter))
    {
    found_end = true;
    }
    else
    {
    TokenNode *current = tree->current;

    if(!is_token_node_nil(current->first))
    { // NOTE: we can assume that if the first node is nil that the last node will be nill aswell
    current->first = node;
    current->last  = node;
    }
    else
    {
    current->last->next = node;

    node->previous = current->last;
    node->parent   = current->parent;

    current->last = node;
    }
    }
    }
    }
    }
    }


    //
    // create tokens and push them onto the tree
    // the  single delimniter acts as the main delimiter. the  final one
    // the sub delimiters are for example commma's you encounter in between
    //
    internal void
    push_token_children(TokenTree *tree, u64 index, String8 delimiter, String8List sub_delimiters)
    {
    b32 found_end = false;
    u8 *buffer    = tree->buffer->data;
    // TODO: for tomorrow cleann this junk up
    while(!found_end)
    {
    for(u64 buffer_idx = index; cstring_is_match(&buffer[buffer_idx], &delimiter); ++buffer_idx)
    { // TODO: iterate until we get the token with the inner loop and when necesary jump the outerloop to the position
    TokenNode *node = {0};
    // consume the token
    // consume while the buffer[index] is not a white space or delimiinter i guess
    for(u8 expected_delimiter_idx = 0; index < expected_delimiters->size; ++index)
    {
    if(cstring_is_match(&expected_delimiters->data[expected_delimiter_idx], &delimiter))
    {
    found_end = true;
    }
    else
    {
    TokenNode *current = tree->current;

    if(!is_token_node_nil(current->first))
    { // NOTE: we can assume that if the first node is nil that the last node will be nill aswell
    current->first = node;
    current->last  = node;
    }
    else
    {
    current->last->next = node;

    node->previous = current->last;
    node->parent   = current->parent;

    current->last = node;
    }
    }
    }
    }
    }
    }


    // create tokens and push them onto the tree
    internal void
    push_token_children(TokenTree *tree, u64 index, String8 delimiter, String8List sub_delimiters)
    {
    b32 found_end = false;
    u8 *buffer    = tree->buffer->data;
    // TODO: for tomorrow cleann this junk up
    while(!found_end)
    {

    for(u64 buffer_idx = index; cstring_is_match(&buffer[buffer_idx], &delimiter); ++buffer_idx)
    { // TODO: iterate until we get the token with the inner loop and when necesary jump the outerloop to the position
    TokenNode *node = {0};
    // consume the token
    // consume while the buffer[index] is not a white space or delimiinter i guess
    for(u8 expected_delimiter_idx = 0; index < expected_delimiters->size; ++index)
    {
    if(cstring_is_match(&expected_delimiters->data[expected_delimiter_idx], &delimiter))
    {
    found_end = true;
    }
    else
    {
    TokenNode *current = tree->current;

    if(!is_token_node_nil(current->first))
    { // NOTE: we can assume that if the first node is nil that the last node will be nill aswell
    current->first = node;
    current->last  = node;
    }
    else
    {
    current->last->next = node;

    node->previous = current->last;
    node->parent   = current->parent;

    current->last = node;
    }
    }
    }
    }
    }
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

    // fill the token node with nil nodes if they are not empty
    {
    FillTokenNodeFieldWithNilNode(next);
    FillTokenNodeFieldWithNilNode(previous);
    FillTokenNodeFieldWithNilNode(child);
    FillTokenNodeFieldWithNilNode(parent);
    FillTokenNodeFieldWithNilNode(first);
    FillTokenNodeFieldWithNilNode(last);
    }
    }
    return;
    }

    #define TokenNodeContainsFlag(node, flag) \
    if(((node->flags) & (flag)) != (0)) \
    return false

    //- lexer

    internal TokenTree *
    lex(MemArena *arena, String8 *buffer)
    {
    TokenTree *tree = PushStruct(arena, TokenTree);

    u64 amount_of_newlines = 0;

    // checks before we start processing the file
    if(buffer->size == 0)
    {
    return NULL; // TODO: Logging Error
    }
    // TODO(nasr):

    // initialize the root node
    {
    #if 0 // FIX: ik ben echt achterlijk dit is een read only data section en ik probeerr ernaar te schrijven...
    tree->root->next = &nil_token_node;
    tree->root->previous = &nil_token_node; // TODO: i think we should later write a function that checks if a node is a parent by checking if the previous node is a nil node
    tree->root->child = &nil_token_node;
    tree->root->parent = &nil_token_node;
    #endif
    // NOTE: but we can still initalize the rest of the tree here

    tree->buffer = buffer; // TODO: don't allocate this. it's a fixed sized buffer. but what we still take the reference of it instead of passing a copy?
    // TODO: watch out for buffer overflows that can overflow in the next data members i guess
    tree->index = 0;
    }

    // nasr: using a unsinged integer index to crash on a negative number
    for(u64 index = *(&tree->index); index < buffer->size; ++tree->index)
    {
    //- skip stuff
    // skip while the current character is classified as whitespace
    if(!cstring_is_whitespace(buffer->data[index]) || !cstring_is_slash(buffer->data[index]))
    continue;
    if(cstring_is_newline(buffer->data[index]))
    {
    ++amount_of_newlines;
    continue;
    }

    TokenNode *node  = PushStruct(arena, TokenNode); // TODO: do we want this to be allocated or not?
    Token      token = {};

    //-;
    if(cstring_is_alpha(buffer->data[index]))
    {
    // if we reach here... current charcter (buffer->data[index]) == alphabetical character
    u64 token_start = index; // this marks the start of the token.
    u64 token_end   = index;

    // at this point we now that the first character isn't a numeric one so we can safely check them both
    // this is because numerics are allowed further into the character but not at the beginnging

    for(; cstring_is_alnum(buffer->data[index]); index)

    //-  node building

    // buid and push node
    {
    //
    // the tree root only takes children. because when we take a look at the tree. the root having a next node means that something is on the same height
    // as that root node. this is only appropriate when working with translation units. i think we could have a forest of trees. where the root nodes/
    // stand for the translation units. and therefor allow the multiple notes to interact with each other.
    //

    // initlaize root  node
    if(is_token_node_nil(tree->root))
    tree->root->child = node;

    u64 lexeme_size          = token_end - token_start; // DEBUG the issue is not with the size when casting to String8
    token.value.string_value = ToString8(&buffer->data[token_start], lexeme_size);

    token.type        = TokenType::Identifier;
    token.line        = amount_of_newlines; // good for line errors...
    token.byte_offset = index * sizeof(u8); // i think? got to figure out later we need this byte offset but i thought it was a good idea so im keeping it:
    };

    // fill the token node with nill nodes
    {
    node->next     = &nil_token_node;
    node->previous = &nil_token_node;
    // TODO: i think we should later write a function // that checks if a node is a parent by checking if the previous node is a nil node
    node->child  = &nil_token_node;
    node->parent = &nil_token_node;
    node->token  = token;
    node->flag   = TokenNodeFlags::None;
    }
    }
    else if(cstring_is_digit(buffer->data[index]))
    {
    // TODO: Error unless the previous token was a declaration token and thus this needs to be handled
    // as a value intead of a declaration

    if(node->parent->token.type == TokenType::Identifier)
    {
    node->parent->flag       = TokenNodeFlags::None;
    node->parent->token.type = TokenType::Identifier;
    }
    else
    {
    node->parent->flag = TokenNodeFlags::Bad;
    }
    }
    else if(buffer->data[index] == '"')
    {
    token.type = TokenType::Identifier;
    {

    for(u64 token_idx = 0; buffer->data[token_idx]; ++token_idx)
    {
    if(cstring_match(buffer->data[token_idx]), '"') // find the closing quotation mark
    {
    token.line         = amount_of_newlines;
    token.byte_offset  = index;
    token.value.string_value =  ToString(buffer->data[index], index - token_idx);
    }

    }
    }
    else if(buffer->data[index] == '\'')
    {
    handle_cpp_style_comments(buffer, index);
    }
    else if(buffer->data[index] == '{')
    {
    String8 closed_param_delim = ToString8("}", 1);

    // an open brace defined a new scope within the thing
    tree->current->next = node;
    node->token.type    = (enum TokenType)'{';
    push_token_children(tree, index, '}', &closed_param_delim);
    }
    else if(buffer->data[index] == '}')
    {
    if(node->first->token.type != (enum TokenType)'{')
    {
    Log("Error: closing brace not matching opening brace");
    }
    else
    {
    Token token         = {};
    tree->current->last = node;

    // TODO: extract into function later
    {
    token.type               = TokenType::Param;
    token.line               = amount_of_newlines;
    token.byte_offset        = index;
    token.value.string_value = ToString8('}', 1);
    }

    if(is_token_node_nil(tree->current->last))
    {
    // fill in the node
    node->token;                        // do we want this as a pointer
    // TODO: What do I do with this? I'm just initalizing the variable without doing anything with it lol...

    node->flag = TokenNodeFlags::Child; // TODO: no flags yet
    #if 0
    node->file_reference.reference_to_root_node = tree->root; // case: we are not the root node so we reference to it to find out in which file we are
    #endif
    node->previous = tree->current;
    node->parent   = node->previous->parent;

    FillTokenNodeFieldWithNilNode(last);
    FillTokenNodeFieldWithNilNode(first);
    FillTokenNodeFieldWithNilNode(child);
    FillTokenNodeFieldWithNilNode(next);
    }
    else
    {
    TokenNode *temp_current_token_node                                   = tree->current->last->next;
    temp_current_token_node->token                                = token;
    temp_current_token_node->flag                                 = TokenType::Identifier;
    temp_current_token_node->file_refernce.reference_to_root_node = tree->root;
    temp_current_token_node->previous                             = tree->last;
    temp_current_token_node->parent                               = tree->last->parent;
    }
    }
    }
    }
    else if(buffer->data[index] == '(')
    {
    if(tree->current->parent)
    {
    push_token_children(tree, index, ,ToString8(')', 1), {0});
    }
    }
    else if(buffer->data[index] == ')')
    {
    if(cstring_is_match(tree->current->first->token.value.string_value.data, ')'))
    { // TODO: fix this ugly thing
    Log("Error: No closing brace found on line  %d amount_of_newlines");
    }
    else
    {
    // TODO: Something I forgot what
    }
    }
    else if(buffer->data[index] == '[') // handle arrays
    {
    //
    // an array consists of children of the variable name
    // so we just do a children until we hit the delimiter
    //

    push_token_children(tree, index, delimiter, ToString8("delimiter",1));
    }
    else if(buffer->data[index] == ']')
    {
    if(tree->current->first->type != (TokenType)'[') Error("didn't close the bracket you moron");
    }
    else if(buffer->data[index] == '#')
    {
    // TODO HANDLE MODULE
    }
    else if(cstring_is_match(buffer->data[index], ':', 2))
    {
    if(tree->current->parent->token.type  == TokenType::Identifier)
    {

    String8List *sub_delimiters = PushStruct(arena, String8List);
    String8Node *sub_colon_node = PushStruct(arena, String8Node);
    String8Node *sub_komma_node = PushStruct(arena, String8Node);


    str8_list_push_node(sub_delimiters, sub_colon_node);
    str8_list_push_node(sub_delimiters, sub_komma_node);

    push_token_children(tree, index, delimiter, sub_delimiters);


    }
    else
    {
    // TODO: Logging
    }
    }
    else
    {
    Log("I think that the colon is misplaced");
    }
    }
    else
    {
    Log("Unrecognized token");
    }

    tree->root->flag = (TokenNodeFlags)(tree->root->flag | TokenNodeFlags::Eof);
    //- end fo file
}

return tree;
}

#endif /* LEXER_H */
