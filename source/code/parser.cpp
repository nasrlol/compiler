internal b32
is_ast_node_nil(AstNode *node)
{
    return (node == NULL || node == &nil_ast_node);

}

// TODO: do we want a reference or copy fo the token we are working on?
internal Ast *
parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree)
{
    ScratchArena parse_scratch = scratch_start(arena);
    Ast *ast = PushStruct(arena, Ast);
    MemStack *stack = arena_to_stack(arena, MiB(10), arena->current_position); 
    
    for(TokenNode *cr = token_tree->current; !is_token_node_nil(cr); cr = cr->next)
    {
        AstNode *new_node = StackPushStruct(stack, AstNode);
        
        FillAstNodeFieldWIthNilNode(new_node, first);
        FillAstNodeFieldWIthNilNode(new_node, last);
        FillAstNodeFieldWIthNilNode(new_node, parent);
        FillAstNodeFieldWIthNilNode(new_node, next);
        
        switch(cr->token->type)
        {
            case TokenType::IdentToken:
            {
                if(is_ast_node_nil(ast->current))
                {
                    ast->current = new_node;
                    ast->current->type = AstNodeType::IdentExpr;
                }
            } break;
            
            case TokenType::Value:
            {
                if(is_ast_node_nil(ast->current->first) &&
		   ast->current->type == AstNodeType::Literal)
                {
                    new_node->type = AstNodeType::Assignment;
                    ast->current = new_node;
                    
                }
            } break;
            
          // this can be taken as a value or parameter
	  // TODO: or do we mark this as an expression and evaluate it later?
	   case TokenType::String: 
            {
                if(ast->current->type == AstNodeType::Function ||
		   ast->current->type == AstNodeType::IdentExpr)
                {
		    // TODO: handle this as an expression
                }
            } break;
            
            case TokenType::Number:
            {
                if(ast->current->type == AstNodeType::Function || ast->current->type == AstNodeType::IdentExpr)
                {
                    
                }
                
            } break;
            
            case TokenType::Double_Equal:
            {
		// swap current node with equal node.
		// first param as first child
		// last param as last child

                // assigning these as binary tells us 

		// TODO: when do we handle expressions?
		new_node->type = AstNodeType::Binary; 

		AstNode *temp_current_node = ast->current;
		ast->current = new_node;
		ast->current->first = temp_current_node;

		if(cr->next->token->type == TokenType::IdentToken) {
		    
		    
		    

		}

            } break;
            
            case TokenType::Greater_Equal:
            {
                
            } break;
            
            case TokenType::Lesser_Equal:
            {
                
            } break;
            
            case TokenType::Param:
            {
                
            } break;
            
            case TokenType::Right_Shift:
            {
                
            } break;
            
            case TokenType::Left_Shift:
            {
                
            } break;
            
            case TokenType::Asterisk:
            {
                
            } break;
            
	    // ignore comments for now
            case TokenType::Cpp_Style_Comment_Start:
            {} break;
            
            case TokenType::C_Style_Comment_Start:
            {} break;
            
            case TokenType::C_Style_Comment_End:
            {} break;
            
            
            default:
            {
                // 1. TokenType::Undedfined
                // 2. null
                
            } break;
            
            
        }
        
        
    }
    
    return NULL;
}

internal void
adopt_ast_node(Ast *ast, AstNode *node)
{
    node->parent = ast->current;
    ast->current->next = node;
}
