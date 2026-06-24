#ifndef EDITOR_PARSER_H
#define EDITOR_PARSER_H

typedef struct Token Token;

typedef enum AstNodeType AstNodeType;
enum AstNodeType
{
 Ast_Node_Literal,
 Ast_Node_Identifier,
 Ast_Node_Binary,
 Ast_Node_Assignment,
 Ast_Node_Return,
 Ast_Node_Function,
 Ast_Node_Undefined,
};

typedef struct SyntaxNode SyntaxNode;
struct SyntaxNode {
 SyntaxNode *first;
 SyntaxNode *last;
 SyntaxNode *parent;
 SyntaxNode *next;

 Token *token;

 AstNodeType type;
};

typedef struct Ast Ast;
struct Ast {
 SyntaxNode *root;
 SyntaxNode *current;
 String8List *symbols;
};

// TODO(nasr): implement this later together with file handling
read_only global_variable
SyntaxNode nil_syntax_node =
{
 &nil_syntax_node,     // first
 &nil_syntax_node,     // last
 &nil_syntax_node,     // parent
 &nil_syntax_node,     // next
 &nil_token,           // token
};

read_only global_variable
ConcreteSyntaxTree nil_concrete_syntax_tree =
{
 &nil_syntax_node,      // root
 &nil_syntax_node,      // currrent
};

#define PeekForward(Node, Type, NilNode) \
 for(; (Node) && (Node) != &(NilNode); (Node) = (Node)->next) \
 if((Node)->token->type == (Type)) \
 break;

#endif // EDITOR_PARSER_H
