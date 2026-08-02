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
    typedef struct TokenTree TokenTree;
    struct TokenTree
    {
    TokenNode *root;
    TokenNode *current;

    String8 *buffer;
    u8 *buffer_position;

    u64 amount_of_new_lines;
    u64 index;
    };

    //- nil variables
    ReadOnly global_variable Token
    nil_token = {
    TokenType::Undefined,
    0,
    {NULL, 0}
    };

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
    {}
    };
    #endif

    #define TokenNodeContainsFlag(node, flag) \
    if(((node->flags) & (flag)) != (0)) \
    return false

    #define FillTokenNodeFieldWithNilNode(field) \
    if(node->field == NULL || !is_token_node_nil(node)) \
    node->field = &nil_token_node;


// - functions
internal b32 is_token_node_nil(TokenNode * node);
internal b32 is_token_nil(Token * token);
internal u64 handle_c_style_comments(TokenTree * tree, u64 start_pos);
internal void handle_cpp_style_comments(String8 * buffer, u64 start_pos);
internal void push_token_children(TokenTree * tree, u64 index, String8List delimiters);
internal void push_token_node(TokenTree * tree, TokenNode * node);
internal TokenTree * tokenize_tree_fmt(MemArena * arena, String8 * buffer_from_file);

#endif /* LEXER_H */
