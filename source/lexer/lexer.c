internal token_tree *
lex(mem_arena *lexical_arena, string8 *buffer)
{
    lexer      *lexer     = PushStruct(lexcial_arena, lexer);

    lexer->text = buffer;

    token_tree *tree      = PushStruct(lexical_arena, token_tree);
    b32         token_end = False;

    // nasr: using a unsinged integer index to crash on a negative number
    for (u32 buffer_index = 0; buffer_index < buffer->size; ++buffer_index)
    {
        s32 jump_index = buffer_index;

        while(is_alpha(buffer[jump_index])) {
            if (token_end == True)
            {
                token_node *node = PushStruct(lexical_arena, token_node);
                break;
            } else {

                ++jump_index;
            }
        }

                // jump to the end to optimize the lexical stuff
        buffer_index = jump_index;
    }

    return tree;
}

internal void
push_token_tree(token_tree *tree, b32 child)
{
}
