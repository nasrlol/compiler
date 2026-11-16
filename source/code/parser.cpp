internal b32 is_ast_node_nil(AstNode *node)
{
	return (node == NULL || node == &nil_ast_node);
}

// used to fill in the AstNode (new_node)
// TokenNode *token_node is the current node in the iteration over the
// token stream produced by the lexer

internal void handle_binary_expression(TokenNode *token_node, AstNode *new_node, Ast *ast)
{
	new_node->type             = AstNodeType::Binary;
	AstNode *current_ast_node = ast->current;
	ast->current->first        = current_ast_node;
	// TODO: ...
	
}

internal void handle_ternary_expression()
{
	/// TODO: do we have any ternary expressions in the language?
}

internal void adopt_ast_node(Ast *ast, AstNode *node)
{
	node->parent       = ast->current;
	ast->current->next = node;
}

// TODO: do we want a reference or copy of the token we are working on?
internal Ast *parse_tokenized_tree(Arena *arena, TokenList *list)
{
	Assert(list);
	Ast      *ast = PushStruct(arena, Ast);
	MemStack *stack;
	stack = ArenaToStack(arena, MiB(10));
	
	for(TokenNode *token_node = list->current; !is_token_node_nil(token_node); token_node = token_node->next) {
		AstNode *new_node = StackPushStruct(stack, AstNode);
		// TODO: cleanup after scan
		if(!new_node) {
			// NOTE: this doesn't need to be part of the compiler output because it should never fail
			Log("Failed to push AST node onto the stack");
			return NULL;
		}
		
		FillAstNodeFieldWIthNilNode(new_node, first);
		FillAstNodeFieldWIthNilNode(new_node, last);
		FillAstNodeFieldWIthNilNode(new_node, parent);
		FillAstNodeFieldWIthNilNode(new_node, next);
		
		switch (token_node->token->type) {
			case TokenType::TokenType_Identifier: {
				if(is_ast_node_nil(ast->current)) {
					ast->current       = new_node;
					ast->current->type = AstNodeType::IdentExpr;
				}
			} break;
			
			case TokenType::TokenType_Keyword: {
				if(is_ast_node_nil(ast->current->first) &&
				   ast->current->type == AstNodeType::Literal) {
					new_node->type = AstNodeType::Assignment;
					ast->current   = new_node;
				}
			} break;
			
			// TODO: or do we mark this as an expression and evaluate it later?
			// this can be taken as a value or parameter
			case TokenType::TokenType_Number: {
				if(ast->current->type == AstNodeType::Function ||
				   ast->current->type == AstNodeType::IdentExpr) {
				}
			} break;
			
			case TokenType::TokenType_Double_Equal: {
				handle_binary_expression(token_node, new_node, ast);
			} break;
			
			case TokenType::TokenType_Greater_Equal: {
			} break;
			
			case TokenType::TokenType_Lesser_Equal: {
			} break;
#if 0
			
			case TokenType::TokenType_Param: {
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
				
				
			} break;
			
			case TokenType::TokenType_Right_Shift: {
			} break;
			
			case TokenType::TokenType_Left_Shift: {
			} break;
			
			case TokenType::TokenType_Asterisk: {
			} break;
			
#endif
			
			default: {
				
			} break;
		}
	}
	
	return NULL;
}
