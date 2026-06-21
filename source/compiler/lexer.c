
internal token_tree *
lex(mem_arena *lexical_arena, string8 *buffer) {

  lexer      *l     = PushStruct(lexical_arena, lexer);

  l->text = buffer;

  token_tree *tree      = PushStruct(lexical_arena, token_tree);
  b32         token_end = False;

  // nasr: using a unsinged integer index to crash on a negative number
  for (u32 buffer_index = 0; buffer_index < buffer->size; ++buffer_index) {
    s32 jump_index = buffer_index;

    while(is_alpha(buffer->data[jump_index])) {
      if (token_end == True) {
        token_node *node = PushStruct(lexical_arena, token_node);
        // push the node
        {
          // 6/21: was here pushing the node


        }
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
