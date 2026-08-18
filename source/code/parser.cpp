internal Ast *
parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree)
{
    Ast *ast = PushStruct(arena, Ast);
    for(TokenNode *cr = token_tree->current_node; !is_nil_token_node(cr); cr = cr->next)
    {
        
        
    }
    
    return NULL;
}