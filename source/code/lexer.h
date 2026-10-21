#ifndef LEXER_H
#define LEXER_H

//-
#define DEBUG_STR8_DIFF(tree, arena, diff_size) \
                                                  debug_str8_diff(tree, arena, (diff_size), __FUNCTION__)

    #define TokenNodeContainsFlag(node, flag) \
    if (((node->flags) & (flag)) != (0))  \
    return false

    #define FillTokenNodeFieldWithNilNode(field)             \
    if (node->field == NULL || !is_token_node_nil(node)) \
    node->field = &nil_token_node;

    enum TokenType {
    // if this is the case. we fall through to use the TokenFlags which do a beter job at specifying
    // the properties of the identifier
    Illegal = 256,
    IdentToken,

    // TODO: what is this commentm man what are you saying
    // because an identfier could really be anything from a value, variable or function.
    Value,

    String,
    Number,
    Double_Equal,
    Greater_Equal,
    Lesser_Equal,
    Param,
    Right_Shift,
    Left_Shift,
    Asterisk,
    Cpp_Style_Comment_Start, // TODO: implement the token type setting for these
    C_Style_Comment_Start,
    C_Style_Comment_End,
    };

    enum TokenNodeFlags : int {
    Invalid = 1,
    Root    = 2,
    Child   = 3,
    Eof     = 4,
    Bad     = 5,
    };

    typedef union TokenValue TokenValue;
    union TokenValue {
    String8 as_str;
    f64     as_float;
    u64     as_int;
    };

    typedef struct Token Token;
    struct Token {
    enum TokenType type;
    u64            line;
    TokenValue     value;
    };

    typedef struct TokenNode TokenNode;
    struct TokenNode {
    TokenNodeFlags flag;
    TokenNode     *next;
    TokenNode     *previous;
    TokenNode     *child;
    TokenNode     *parent;
    TokenNode     *first;
    TokenNode     *last;
    Token         *token;
    u64            buffer_position;
    };

    typedef struct Lexeme Lexeme;
    struct Lexeme {
    String8 *buffer;
    u64      index;
    };

    // Find a way to do this without allocating the tokens on the heap. It's a huge toll on the
    // performance of the program
    typedef struct TokenTree TokenTree;
    struct TokenTree {
    TokenNode *root;
    TokenNode *current;
    String8   *buffer;
    u64        amount_of_new_lines;
    u64        index;
    };

    typedef struct TranslationUnit TranslationUnit;
    struct TranslationUnit {
    // stored as an array
    // per omdule one unit, to handle namespace issues
    TokenTree *tree_units;
    };


    //- nil variables
    ReadOnly global_variable Token nil_token = {TokenType::Illegal, 0, {}};

    ReadOnly global_variable TokenNode nil_token_node = {TokenNodeFlags::Invalid,
    &nil_token_node, // next
    &nil_token_node, // previous
    &nil_token_node, // child
    &nil_token_node, // parent
    &nil_token_node, // first
    &nil_token_node, // last
    {}};


    ReadOnly TokenTree nil_token_tree{

    &nil_token_node, // root
    &nil_token_node, // current;
    {},              //*buffer;
    0,               // amount_of_new_lines
    0,               // index;
};


typedef struct TokenNodeList TokenNodeList;
struct TokenNodeList{
    TokenNode *current;
    TokenNode *root;
};


// - functions
internal b32        is_token_node_nil(TokenNode *node);
internal b32        is_token_nil(Token *token);
internal void       push_token_children(MemArena *arena, TokenTree *tree, String8List *delimiters);
internal void       push_token_node(TokenTree *tree, TokenNode *node);
internal TokenTree *tokenize_tree_fmt(MemArena *arena, String8 *buffer_from_file);
internal Token      consume_token(String8 *buffer);
internal TokenNode  create_node_from_token();

internal b32 is_token_node_nil(TokenNode *node) { return (node == &nil_token_node); }
internal b32 is_token_nil(Token *token) { return (token == &nil_token); }

#endif /* LEXER_H */
