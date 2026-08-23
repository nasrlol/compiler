#ifndef EDITOR_PARSER_H
#define EDITOR_PARSER_H

// typedef enum AstNodeType AstNodeType;
enum AstNodeType {
    Literal,
    Identifier,
    Binary,
    Assignment,
    Return,
    Function,
    Undefined,
};

typedef struct AstNode AstNode;
struct AstNode {
    AstNode *first;
    AstNode *last;
    
    AstNode *parent;
    AstNode *next;
    AstNodeType *type;
};

typedef struct Ast Ast;
struct Ast {
    AstNode     *root;
    AstNode     *current;
    String8List *symbols;
};

ReadOnly global_variable
AstNode nil_ast_node = {
    &nil_syntax_node, // first
    &nil_syntax_node, // last
    &nil_syntax_node, // parent
    &nil_syntax_node, // next
};

ReadOnly global_variable
Ast nil_ast = {
    &nil_syntax_node, // root
    &nil_syntax_node, // currrent
};

#define PeekForward(node, type, nil_node) \
for(; (node) && (node) != &(nil_node); (node) = (node)->next) \
if((node)->token->type == (type)) \
break;

//-
internal Ast *parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree);

//- syntax
global_variable
String8 reserved_words[] = {
    
	str8("struct"),
	str8("while"),
	str8("return"),
	str8("for"),
	str8("import"),
	str8("switch"),
	str8("case"),
	str8("defer"),
	str8("enum"),
};


internal b32 is_ast_node_nil(AstNode *node);

#define FillAstNodeFieldWIthNilNode(field)		    \
if(node->field == NULL || !is_ast_node_nil(node)) \
node->field = &nil_ast_node;


#endif // EDITOR_PARSER_H