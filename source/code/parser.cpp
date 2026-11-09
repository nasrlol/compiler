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
	
	// TODO: i think we should categorize these better in the future
	if(token_node->next->token->type == TokenType::IdentToken ||
	   token_node->next->token->type == TokenType::Number ||
	   token_node->next->token->type == TokenType::String) {
		TokenNode *next_token_node = token_node->next;
		token_node->last    = next_token_node;
		// restructure the tree by moving the next node to sit closer?
		
		// todo: this needs testing
		token_node->next = token_node->next->next;
	}
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
internal Ast *parse_tokenized_tree(Arena *arena, TokenTree *token_tree)
{
	// TODO: replace with something containing a breakpoint, Assert or something like that
	// currently it still throws that asm junk
	// i'm going to ignore it for now and just put if checks. do a scan of the code later and fix it
	// up
	if(!token_tree || !arena)
		return NULL;
	
	Ast      *ast = PushStruct(arena, Ast);
	MemStack *stack;
	stack = ArenaToStack(arena, MiB(10));
	
	for(TokenNode *token_node = token_tree->current; !is_token_node_nil(token_node); token_node = token_node->next) {
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
			case TokenType::IdentToken: {
				if(is_ast_node_nil(ast->current)) {
					ast->current       = new_node;
					ast->current->type = AstNodeType::IdentExpr;
				}
			} break;
			
			case TokenType::Value: {
				if(is_ast_node_nil(ast->current->first) &&
				   ast->current->type == AstNodeType::Literal) {
					new_node->type = AstNodeType::Assignment;
					ast->current   = new_node;
				}
			} break;
			
			// TODO: or do we mark this as an expression and evaluate it later?
			// this can be taken as a value or parameter
			case TokenType::String: {
				if(ast->current->type == AstNodeType::Function ||
				   ast->current->type == AstNodeType::IdentExpr) {
					// TODO: handle this as an expression
				}
			} break;
			
			case TokenType::Number: {
				if(ast->current->type == AstNodeType::Function ||
				   ast->current->type == AstNodeType::IdentExpr) {
				}
				
			} break;
			
			case TokenType::Double_Equal: {
				handle_binary_expression(token_node, new_node, ast);
			} break;
			
			case TokenType::Greater_Equal: {
			} break;
			
			case TokenType::Lesser_Equal: {
			} break;
			
			case TokenType::Param: {
			} break;
			
			case TokenType::Right_Shift: {
			} break;
			
			case TokenType::Left_Shift: {
			} break;
			
			case TokenType::Asterisk: {
			} break;
			
			// ignore comments for now
			case TokenType::Cpp_Style_Comment_Start: {
			} break;
			
			case TokenType::C_Style_Comment_Start: {
			} break;
			
			case TokenType::C_Style_Comment_End: {
			} break;
			
			default: {
				// 1. TokenType::Undefined
				// 2. null
				
			} break;
		}
	}
	
	return NULL;
}
