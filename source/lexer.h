#ifndef LEXER_H
#define LEXER_H

//-
enum struct TokenType {
 // first 255 characters are reserved for ascii stuff
 Token_Undefined= 256,
 Token_Identifier,  // if this is the case. we fall through to use the TokenFlags which do a beter job at specifying the properties of the identifier
 // because an identfier could really be anything from a value, variable or function.
 Token_Value,

 Token_String,
 Token_Number,

 Token_Double_Equal,
 Token_Greater_Equal,
 Token_Lesser_Equal,
 Token_Param,

 Token_Return,

 Token_If,
 Token_Else,
 Token_For,
 Token_While,

 Token_Break,
 Token_Continue,

 Token_Right_Shift,
 Token_Left_Shift,
 Token_Asterisk,

 Token_Cpp_Style_Comment_Start,
 Token_C_Style_Comment_Start,
 Token_C_Style_Comment_End,
};

enum struct TokenFlags {
 Flag_None        = (0 << 0),
 Flag_Constant    = (1 << 0),
 Flag_Global      = (1 << 1),
 Flag_Value       = (1 << 2),
 Flag_Definition  = (1 << 3),
 Flag_Comparison  = (1 << 4),
 Flag_Deprecated  = (1 << 5),
 Flag_Dirty       = (1 << 6),
 Flag_FuncBody    = (1 << 7),
 Flag_Expression  = (1 << 8),
 Flag_Func        = (1 << 9),
 Flag_Declaration = (1 << 10),

 Flag_RootNode    = (1 << 11),
 Flag_Error       = (1 << 32),
};

typedef struct Token Token;
struct Token {
 enum TokenType type;
 TokenFlags flags;
 s32        line; // good for line errors...
 s32        column;
 u64        byte_offset;

 union {

  String8 string_value;
  f64     float64_value; // this is a seperate type because floats have a different notation than integers do
  u64     integer_value;
 };
};

typedef struct TokenNode TokenNode;
struct TokenNode {


 union {
  TokenNode *reference_to_root_node; // case: we are not the root node so we reference to it to find out in which file we are
  String8 file_name;                 // case: we are the root node and we just store the name? @TODO of the file? the file or the path?
 } file_reference;

 TokenNode *next;
 TokenNode *previous;

 TokenNode *child;
 TokenNode *parent;

 Token      token; // do we want this as a pointer?
};


// find a way to do this without allocating the tokens on the heap. It's a huge toll on the performance of the program
typedef  struct TokenTree {

 TokenNode *root;
 TokenNode *current;

 String8 *text_buffer;    // @TODO: don't allocate this. it's a fixed sized buffer. but what we still take the reference of it instead of passing a copy?
 u8 *buffer_position;
} TokenTree;

//- nil variables
read_only global_variable
Token nil_token = { TokenType::Token_Undefined, TokenFlags::Flag_None, 0, 0, 0, {NULL, 0}};

read_only global_variable
TokenNode nil_token_node = { &nil_token_node, &nil_token_node, NULL };

//- nil variable functions
internal b32
is_token_node_nil(TokenNode *node) {
 return (node == &nil_token_node);
}

internal b32
is_token_nil(Token *token) {
 return (token == &nil_token);
}

//- token tree helper functions

internal void
push_token_node(TokenTree tree, TokenNode node) {



}

//- lexer

internal TokenTree *
lex(MemArena *arena, String8 *buffer) {

 TokenTree *tree      = PushStruct(arena, TokenTree);

 u64 amount_of_newlines  = 0;


 // checks before we start processing the file
 if(buffer->size == 0) {
  return NULL; // @TODO: Logging Error
 }

 // initialize the root node
 {
  tree->root->next     = &nil_token_node;
  tree->root->previous = &nil_token_node; // @TODO: i think we should later write a function that checks if a node is a parent by checking if the previous node is a nil node
  tree->root->child    = &nil_token_node;
  tree->root->parent   = &nil_token_node;

  // zero initializers don't work like this in cpp std 11?
  // so you bassically can't do `some_struct_variable = {0};`
  tree->root->token    = {};
 }

 // nasr: using a unsinged integer index to crash on a negative number
 for (u8 index = 0; index < buffer->size; ++index) {

  //- skip stuff
  // skip while the current character is classified as whitespace
  if(!is_whitespace(buffer->data[index]) || !is_slash(buffer->data[index])) continue;
  if (is_newline(buffer->data[index])) {
   ++amount_of_newlines;
   continue;
  }

  TokenNode *node = PushStruct(arena, TokenNode); // @TODO(nasr): do we want this to be allocated or not?
  Token     token  =  {};


  //-
  if(is_alpha(buffer->data[index])) {

   // if we reach here... current charcter (buffer->data[index]) == alphabetical character
   s32 token_start = index; // this marks the start of the token.
   s32 token_end = index;

   // at this point we now that the firt character isn't a numeric one so we can safely check them both
   // this is because numerics are allowed further into the character but not at the beginnging

   u64 temp_index = index;
   while(is_alpha_num(buffer->data[index])) temp_index++; // go to the end of the character

   //-  node building

   // buid and push node
   {
    //
    // the tree root only takes children. because when we take a look at the tree. the root having a next node means that something is on the same height
    // as that root node. this is only appropriate when working with translation units. i think we could have a forest of trees. where the root nodes/
    // stand for the translation units. and therefor allow the multiple notes to interact with each other.
    //

    // initlaize root  node
    if(is_token_node_nil(tree->root))  tree->root->child = node ;

    u64 lexeme_size = token_end - token_start; // @debug the issue is not with the size when casting to String8
    token.string_value = ToString8(buffer->data[token_start], lexeme_size);

    token.type = TokenType::Token_Identifier;
    token.flags = TokenFlags::Flag_None;
    token.line = amount_of_newlines; // good for line errors...
    token.column = index - amount_of_newlines;
    token.byte_offset = index * sizeof(u8); // i think? got to figure out later we need this byte offset but i thought it was a good idea so im keeping it:
   };

   // fill the token node with nill nodes
   {
    node->next     = &nil_token_node;
    node->previous = &nil_token_node;
    // @TODO: i think we should later write a function // that checks if a node is a parent by checking if the previous node is a nil node
    node->child    = &nil_token_node;
    node->parent   = &nil_token_node;
    node->token    = token;
   }


  } else if(is_digit(buffer->data[index])) {
   // @TODO: Error unless the previous token was a declaration token and thus this needs to be handled
   // as a value intead of a declaration

   if(node->parent.type == TokenType::Token_Identifier
      || node->parent.flags |= TokenFlags::Flag_Definition
      || node->parent.flags |= TokenFlags::Flag_Declaration) {


    node->parent.flags = Flag_Value;
    node->parent.type  = Token_Identifier;

   } else {
    node->parent.flags |= TokenFlags::Flag_Error;
   }

  } else if(buffer->data[index] == '"') {

   token.type = TokenType::Token_Identifier;
  }

  else if(buffer->data[index] == '\'') {
   // tokens_handle_cpp_style_comments();

  } else if(buffer->data[index] == '{') {

   node->

  } else if(buffer->data[index] == '}') {

  } else if(buffer->data[index] == '(') {
  } else if(buffer->data[index] == ')') {

  } else if(buffer->data[index] == '[') {
  } else if(buffer->data[index] == ']') {

  } else if(buffer->data[index] == '#') {
   // @TODO HANDLE MODULE
  } else if(buffer->data[index] == ';') {

  } else if(buffer->data[index] == ':') {

  } else {
   // @TODO: Logging
  }


  //-
 }

 return NULL ;
}


#endif /* LEXER_H */
