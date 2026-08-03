#ifndef EDITOR_PARSER_H
#define EDITOR_PARSER_H

// typedef enum AstNodeType AstNodeType;
enum AstNodeType {
    Ast_Node_Literal,
    Ast_Node_Identifier,
    Ast_Node_Binary,
    Ast_Node_Assignment,
    Ast_Node_Return,
    Ast_Node_Function,
    Ast_Node_Undefined,
};

typedef struct AstNode AstNode;
struct AstNode {
    AstNode *first;
    AstNode *last;

    AstNode *parent;
    AstNode *next;
};

typedef struct Ast Ast;
struct Ast {
    AstNode     *root;
    AstNode     *current;
    String8List *symbols;
};

ReadOnly global_variable
AstNode nil_syntax_node = {
    &nil_syntax_node, // first
    &nil_syntax_node, // last
    &nil_syntax_node, // parent
    &nil_syntax_node, // next
};

ReadOnly global_variable
Ast nil_concrete_syntax_tree = {
    &nil_syntax_node, // root
    &nil_syntax_node, // currrent
};

#define PeekForward(node, type, nil_node) \
    for(; (node) && (node) != &(nil_node); (node) = (node)->next) \
    if((node)->token->type == (type)) \
    break;

//-
internal Ast *parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree);


#endif // EDITOR_PARSER_H