internal Ast *
parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree)
{
    Ast *ast = PushStruct(arena, Ast);
    for(TokenNode *cr = token_tree->current; !is_token_node_nil(cr); cr = cr->next)
    {
        
        
    }
    
    return NULL;
}