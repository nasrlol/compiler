#ifndef EDITOR_PARSER_H
#define EDITOR_PARSER_H

// typedef enum AstNodeType AstNodeType;
enum AstNodeType {
	Literal,
	IdentExpr, // TODO: evaluate this in an expression pass
	Binary,
	Assignment,
	Return,
	Function,
};

typedef struct AstNode AstNode;
struct AstNode {
	AstNode    *first;
	AstNode    *last;
	AstNode    *parent;
	AstNode    *next;
	AstNodeType type;
};

typedef struct Ast Ast;
struct Ast {
	AstNode     *root;
	AstNode     *current;
	String8List *symbols;
};

ReadOnly global_variable AstNode nil_ast_node = {
	&nil_ast_node, // first
	&nil_ast_node, // last
	&nil_ast_node, // parent
	&nil_ast_node, // next
};

ReadOnly global_variable Ast nil_ast = {
	&nil_ast_node, // root
	&nil_ast_node, // current
};

#define PeekForward(node, type, nil_node)                          \
for(;(node) && (node) != &(nil_node); (node) = (node)->next)  \
if((node)->token->type == (type))                              \
break;

#define FillAstNodeFieldWIthNilNode(node, field)       \
if(node->field == NULL || !is_ast_node_nil(node)) \
node->field = &nil_ast_node;


#endif // EDITOR_PARSER_H
