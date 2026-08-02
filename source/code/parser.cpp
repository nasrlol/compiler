internal Ast *
               parse_tokenized_tree_fmt(MemArena *arena, TokenTree *token_tree)
    {
    Ast *ast = PushStruct(arena, Ast);
    for(TokenNode *current = token_tree->root; !(current->flag | TokenNodeFlags::Eof); current = current->next)
    {

    }

    return NULL;
}