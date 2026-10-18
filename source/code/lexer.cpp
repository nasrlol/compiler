// TODO: replace the logs with this
internal void debug_str8_diff(TokenTree *tree, MemArena *arena, u64 diff_size,
                              const char *function_name)
{
 ScratchArena scratch = scratch_start(arena);
 String8 debug_log    = PushString8(arena, diff_size);
 Unused(debug_log);
 debug_log.data       = tree->buffer->data;
 debug_log.size       = diff_size;
 Log("call: %s \t" Str8Fmt "\n", function_name, Str8Arg(debug_log));
 scratch_end(&scratch);
}

// create tokens, from a certain starting position until a certain
// delimiter. build the tokens and push them onto the tree
internal void push_token_children(MemArena *arena, TokenTree *tree, String8List *delimiters)
{
 String8 *temp_buffer = PushStruct(arena, String8);
 MemCpy(temp_buffer, tree->buffer, tree->buffer->size);

 FOR_STR8_NODE(delimiters, curr_delimiter)
 {
  FOR_STR8_MATCH(*temp_buffer, curr_delimiter->value)
  {
   TokenNode *node    = PushStruct(arena, TokenNode);
   TokenNode *current = tree->current;

   // NOTE: we can assume that if the first node is nil that the last node will be nill
   // aswell
   if (!is_token_node_nil(current->first)) {
    current->first = node;
    current->last  = node;
   } else {
    node->previous      = current->last;
    node->parent        = current->parent;
    current->last->next = node;
    current->last       = node;
   }
  }
 }
}

// TODO: fix this function

internal void push_token_node(TokenTree *tree, TokenNode *node)
{
 if (is_token_node_nil(tree->root)) {
  tree->root    = node;
  tree->current = node;
  return;
 } else {
  tree->current->previous = tree->current;
  tree->current->next = node;
  node->parent = tree->current;
  tree->current = node;

  FillTokenNodeFieldWithNilNode(next);
  FillTokenNodeFieldWithNilNode(previous);
  FillTokenNodeFieldWithNilNode(child);
  FillTokenNodeFieldWithNilNode(parent);
  FillTokenNodeFieldWithNilNode(first);
  FillTokenNodeFieldWithNilNode(last);
 }
}

internal TokenTree *tokenize_tree_fmt(MemArena *arena, String8 *buffer_from_file)
{
 b32 is_empty = str8_is_empty(buffer_from_file);
 if (is_empty) {
  Log("No buffer passed as argument");
  Assert(0);
 }

 TokenTree *tree = PushStruct(arena, TokenTree);
 Assert(tree);

 tree->buffer = PushStruct(arena, String8);

 MemCpy(tree->buffer, buffer_from_file, buffer_from_file->size);

 tree->buffer->size = buffer_from_file->size;
 tree->index        = 0;

 FOR_STR8_CHOP_LEFT(index, (*tree->buffer), 1)
 {
  u8 current_character = tree->buffer->data[0];

  // Handle skipping
  b32 is_whitespace = cstring_is_whitespace(current_character);
  if (is_whitespace) {
   continue;
  }

  b32 is_newline = cstring_is_newline(current_character);
  if (is_newline) { // NOTE: do one extra iteration to skip the new line. we keep getting
   // stuck on the current character otherwse
   tree->amount_of_new_lines += 1;
   str8_chop_left_ptr(tree->buffer, 1); // TODO: dit mag niet, je mag de oorspronkelijke buffer niet aanpassen. text transformations zijn enkel toegelaten in de preprocessor
   continue;
  }

  TokenNode *node         = PushStruct(arena, TokenNode);
  Token *token            = PushStruct(arena, Token);
  String8List *delimiters = PushStruct(arena, String8List);

  if (cstring_is_alpha(current_character)) {
   u64 index = 0;

   // TODO: iterate and check if the character is ???
#if 0
   baseString8 *temp_buffer = PushString8(arena, buffer->size;);

   b32 is_alpha = cstring_is_alpha_num(current_character);
   FOR_STR8_ARB_FN(is_alpha, str8_chop_left, *temp_buffer, 1)
   {
    cstring_is_alpha_num(temp_buffer);
    ++index;
   }
#endif

   // 8/2/2026 TODO: but we don"t initialize the root node at all?
   // we talk about the children but we ignore the root node
   // and what is the point of focussing on this here when we have a function that handles
   // this?

   // the tree root only takes children. because when we take a look at the tree. the root
   // having a next node means that something is on the same height as that root node. this
   // is only appropriate when working with translation units. i think we could have a
   // forest of trees. where the root nodes/ stand for the translation units. and therefor
   // allow the multiple notes to interact with each other. initialize root node
   if (is_token_node_nil(tree->root)) {
    tree->root->child = node;
   }
   ////////////////////////////////

   // TODO: try replacign this with an assertion a bit earlier than this
   if (token) {
    token->value.as_str = str8_chop_left(*tree->buffer, index);
    token->type         = TokenType::IdentToken;
   }
  }

  else if (cstring_is_digit(current_character)) {
   if (node->parent->token->type == TokenType::IdentToken) {
    node->parent->flag        = (TokenNodeFlags)0;
    node->parent->token->type = TokenType::IdentToken;
   } else {
    node->parent->flag = TokenNodeFlags::Bad;
    break;
   }
  } else if (current_character == '{') {

   tree->current->next           = node;
   node->token->type             = (enum TokenType)'{';
   String8Node right_paran_delim = str8_node("}");
   str8_list_push_node(delimiters, &right_paran_delim);

  } else if (current_character == '\"') {

   // token->type == TokenType:IdentToken;

  } else if (current_character == '}') {
   if (node->first->token->type != (enum TokenType)'{') {
    Log("Error: closing brace not matching opening brace\n");
   } else {
    Token *token        = PushStruct(arena, Token);
    tree->current->last = node;

    token->type         = TokenType::Param;
    token->line         = tree->amount_of_new_lines;
    token->value.as_str = str8("}");

    if (is_token_node_nil(tree->current->last)) {
     node->token    = token;
     node->flag     = TokenNodeFlags::Child;
     node->previous = tree->current;
     node->parent   = node->previous->parent;

     FillTokenNodeFieldWithNilNode(last);
     FillTokenNodeFieldWithNilNode(first);
     FillTokenNodeFieldWithNilNode(child);
     FillTokenNodeFieldWithNilNode(next);

    } else {

     TokenNode *temp_current_token_node = tree->current->last->next;
     temp_current_token_node->token     = token;
     temp_current_token_node->flag      = TokenNodeFlags::Invalid;
     temp_current_token_node->previous  = tree->current->last;
     temp_current_token_node->parent    = tree->current->last->parent;
    }
   }
  } else if (current_character == '(') {
   if (tree->current->parent) {
    String8Node *right_paran = PushStruct(arena, String8Node);
    *right_paran             = str8_node(")");
    str8_list_push_node(delimiters, right_paran);
    push_token_children(arena, tree, delimiters);
    push_token_node(tree, node);
   }
  } else if (current_character == ')') {
   // TODO: fix this ugly thing
   if (str8_is_match(tree->current->first->token->value.as_str, str8(")"))) {
    push_token_node(tree, node);
   } else {
    Log("Error: No opening brace found on line %llu", tree->amount_of_new_lines);
   }
  }
  // NOTE: handle arrays --  an array consists of children of the variable name
  // so we just do a children until we hit the delimiter
  else if (current_character == '[') {
   String8List *delimiters    = PushStruct(arena, String8List);
   String8Node *right_bracket = PushStruct(arena, String8Node);
   *right_bracket             = str8_node("]");
   str8_list_push_node(delimiters, right_bracket);
   push_token_node(tree, node);
  } else if (current_character == ']') {
   if (tree->current->first->token->type != (enum TokenType)'[') {
    Log("didn't close the bracket you moron");
   }
  } else if (current_character == ':') {
   if (tree->current->parent &&
       tree->current->parent->token->type == TokenType::IdentToken) {

    String8List *delimiters = PushStruct(arena, String8List);
    String8Node *colon_node = PushStruct(arena, String8Node);
    String8Node *komma_node = PushStruct(arena, String8Node);

    str8_list_push_node(delimiters, colon_node);
    str8_list_push_node(delimiters, komma_node);
    push_token_children(arena, tree, delimiters);
   } else {
    // TODO: push a warning / error
    Log("syntax error. Issue with the previous token. Unexpected token.");
   }
  } else {
   breakpoint();
   PANIC("Not valid syntax");
  }
 }

 return tree;
}
