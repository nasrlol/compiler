// TODO: do we want a reference or copy fo the token we are working on?
internal Ast *
parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree)
{
    ScratchArena *parse_scratch = scratch_start(arena);
    MemStack *stack = StackCreate(MiB(100));
    Ast *ast = PushStruct(arena, Ast);
    
    for(TokenNode *cr = token_tree->current; !is_token_node_nil(cr); cr = cr->next)
    {
        AstNode *new_node = PushStruct(arena, AstNode);
        FillAstNodeFieldWIthNilNode(new_node, first);
        FillAstNodeFieldWIthNilNode(new_node, last);
        FillAstNodeFieldWIthNilNode(new_node, parent);
        FillAstNodeFieldWIthNilNode(new_node, next);
        
        switch(cr->type)
        {
            case TokenType::Identifier:
            {
                if(is_ast_node_nil(ast->current))
                {
                    ast->current = new_node;
                    ast->current->type = AstNodeType::Identifier;
                }
            } break;
            
            case TokenType::Value:
            {
                if(is_ast_node_nil(ast->current->first) && ast->current->type == AstNodeType::PLiteral)
                {
                    ast->current = new_node;
                }
            } break;
            
            case TokenType::String:
            {
                if(ast->current->type == AstNodeType::PLiteral)
                {
                    
                    
                }
            } break;
            
            case TokenType::Number:
            {
                if(ast->current->type == AstNodeType::PLiteral)
                {
                    
                }
                
            } break;
            
            case TokenType::Double_Equal:
            {
                
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
            
            case TokenType::Cpp_Style_Comment_Start:
            {
                
            } break;
            
            case TokenType::C_Style_Comment_Start:
            {
                
            } break;
            
            case TokenType::C_Style_Comment_End:
            {
                
            } break;
            
            
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