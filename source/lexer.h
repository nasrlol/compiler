#ifndef LEXER_H
#define LEXER_H

//-

// @TODO: find a fix for this continuing casting to enum
enum struct TokenType {
    // first 255 characters are reserved for ascii stuff
    Undefined= 256,
    Identifier,  // if this is the case. we fall through to use the TokenFlags which do a beter job at specifying the properties of the identifier

    Value, // because an identfier could really be anything from a value, variable or function.

    String,
    Number,

    Double_Equal,
    Greater_Equal,
    Lesser_Equal,
    Param,

    Right_Shift,
    Left_Shift,
    Asterisk,

    Cpp_Style_Comment_Start,
    C_Style_Comment_Start,
    C_Style_Comment_End,
};

enum TokenNodeFlags  {
    None    = (1 << 0),
    Root    = (1 << 11),
    Child   = (1 << 12),
    Eof     = (1 << 13),
    Bad     = (1 << 14),
};

typedef struct Token Token;
struct Token {
    enum TokenType type;
    s32        line; // good for line errors...
    s32        column;
    u64        byte_offset;

    union {

        String8 string_value;
        f64     float64_value; // this is a seperate type because floats have a different notation than integers do
        u64     integer_value;
    } value;
};

typedef struct TokenNode TokenNode;
struct TokenNode {

    TokenNodeFlags flag;

#if 0
    union {
        TokenNode *reference_to_root_node; // case: we are not the root node so we reference to it to find out in which file we are
        String8 file_name;                 // case: we are the root node and we just store the name? @TODO of the file? the file or the path?
    } file_reference;
#endif

    TokenNode *next;
    TokenNode *previous;

    TokenNode *child;
    TokenNode *parent;

    TokenNode *first;
    TokenNode *last;

    Token      token; // do we want this as a pointer?
};


// find a way to do this without allocating the tokens on the heap. It's a huge toll on the performance of the program
typedef  struct TokenTree {

    TokenNode *root;
    TokenNode *current;

    String8 *buffer;    // @TODO: don't allocate this. it's a fixed sized buffer. but what we still take the reference of it instead of passing a copy?
    u8 *buffer_position;
    u64 index;
} TokenTree;

//- nil variables
ReadOnly global_variable Token
nil_token = { TokenType::Undefined,  0, 0, 0, {NULL, 0}};

#if 0
ReadOnly global_variable TokenNode
nil_token_node = {
    TokenNodeFlags::None, // TokenNodeFlags
    {NULL},    // nill referecnce for *reference_to_root_node

    NULL,      // next
    NULL,      // previous

    NULL,      // child
    NULL,      // parent

    NULL,      // first
    NULL,      // last

    {}                   // token

};
#endif

#if 1
ReadOnly global_variable TokenNode
nil_token_node = {

    TokenNodeFlags::None,
#if 0
    &nil_token_node,        // file_reference
#endif
    &nil_token_node,        // next
    &nil_token_node,        // previous
    &nil_token_node,        // child
    &nil_token_node,        // parent
    &nil_token_node,        // first
    &nil_token_node,        // last
    {}
};
#endif

// @TODO: rmeove this
internal void
fill_in_nil_token_node(TokenNode *nil_node) {

#if 0
    nil_node->next           =  &nil_token_node;      // next
    nil_node->previous       =  &nil_token_node;      // previous
    nil_node->child          =  &nil_token_node;      // child
    nil_node->parent         =  &nil_token_node;      // parent
    nil_node->first          =  &nil_token_node;      // first
    nil_node->last           =  &nil_token_node;      // last
    nil_node->token          =  {};                   // token,
#endif

}

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


#define FillTokenNodeFieldWithNilNode(field) if(node->field == NULL || !is_token_node_nil(node)) node->field = &nil_token_node;

#if 0


internal void
fill_token_node_with_nil_nodes(TokenNode *node) {
    if(node->next == NULL)     node->next     = &nil_token_node;
    if(node->previous == NULL) node->previous = &nil_token_node;
    if(node->child == NULL)    node->child    = &nil_token_node;
    if(node->parent == NULL)   node->parent   = &nil_token_node;
    if(node->first == NULL)    node->first    = &nil_token_node;
    if(node->last == NULL)     node->last     = &nil_token_node;
}
#endif



internal void
tokens_handle_cpp_style_comments() {

    // @TODO: handle comments
    // for each line that starts with // or /*

    return;
}



#define PushTokenChildren(tree, delimiter, expected_delimiters) push_token_children(tree, tree->buffer, tree->index, (u8 *)delimiter, expected_delimiters)

// create tokens and push them onto the tree
internal void
push_token_children(TokenTree *tree, String8 *buffer, u64 index, u8 *delimiter, String8 expected_delimiters) {





  for(u64 temp_index = index; buffer->data[temp_index] != *delimiter; ++temp_index) { // @TODO: iterate until we get the token with the inner loop and when necesary jump the outerloop to the position



    TokenNode *node = {0};



      // consume the token 
      // consume while the buffer[index] is not a while space or delimiter i guess
      {


	for(u64 temp_index = index; !is_whitespace_or_delimiter(buffer->data[temp_index], expected_delimiters.data); ++temp_index) { 

	  for(u8 index = 0; index < expected_delimiters.size; ++index) {


	  }




	}

      }

      TokenNode *current = tree->current;

        if(!is_token_node_nil(current->first)) { // @note: we can assume that if the first node is nil that the last node will be nill aswell
            current->first = node;
            current->last  = node;
        } else {

            current->last->next = node;

            node->previous = current->last;
            node->parent = current->parent;

            current->last = node;


            // @TODO: i think this is good but have to check again
            // 1. fix the next on the last one
            // 2. make the last point to that node

        }


        // @TODO: find a mechenism to make sure we aren't going to far.
    }

    return;
}

internal void
push_token_node(TokenTree *tree, TokenNode *node) {

    if(is_token_node_nil(tree->root)) {
        tree->root        = node;
        tree->current     = node;
        return;
    } else  {
        tree->current->next = node;
        node->parent = tree->current;

        // fill the token node with nil nodes if they are not empty
        {
            FillTokenNodeFieldWithNilNode(next);
            FillTokenNodeFieldWithNilNode(previous);
            FillTokenNodeFieldWithNilNode(child);
            FillTokenNodeFieldWithNilNode(parent);
            FillTokenNodeFieldWithNilNode(first);
            FillTokenNodeFieldWithNilNode(last);
        }

    }
    return;
}


#define TokenNodeContainsFlag(node, flag) if(((node->flags) & (flag)) != (0)) return false

//- lexer

internal TokenTree *
lex(MemArena *arena, String8 *buffer) {

    TokenTree *tree      = PushStruct(arena, TokenTree);

    u64 amount_of_newlines  = 0;


    // checks before we start processing the file
    if(buffer->size == 0) {
        return NULL; // @TODO: Logging Error
    }
    // TODO(nasr):

    // initialize the root node
    {

#if 0 // @FIXED ik ben echt achterlijk dit is een read only data section en ik probeerr ernaar te schrijven...
        tree->root->next     = &nil_token_node;
        tree->root->previous = &nil_token_node; // @TODO: i think we should later write a function that checks if a node is a parent by checking if the previous node is a nil node
        tree->root->child    = &nil_token_node;
        tree->root->parent   = &nil_token_node;
#endif

	// @NOTE: but we can still initalize the rest of the tree here

    tree->buffer = buffer;    // @TODO: don't allocate this. it's a fixed sized buffer. but what we still take the reference of it instead of passing a copy?
    // @TODO: watch out for buffer overflows that can overflow in the next data members i guess
    tree->index = 0;
    
    }

    // nasr: using a unsinged integer index to crash on a negative number
    for (u64 index = *(&tree->index) ; index < buffer->size; ++tree->index) {

        //- skip stuff
        // skip while the current character is classified as whitespace
        if(!is_whitespace(buffer->data[index]) || !is_slash(buffer->data[index])) continue;
        if (is_newline(buffer->data[index])) {
            ++amount_of_newlines;
            continue;
        }

        TokenNode *node = PushStruct(arena, TokenNode); // @TODO: do we want this to be allocated or not?
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
                token.value.string_value = ToString8(buffer->data[token_start], lexeme_size);

                token.type = TokenType::Identifier;
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
                node->flag     = TokenNodeFlags::None;
            }


        } else if(is_digit(buffer->data[index])) {
            // @TODO: Error unless the previous token was a declaration token and thus this needs to be handled
            // as a value intead of a declaration

            if(node->parent->token.type == TokenType::Identifier) {

                node->parent->flag       = TokenNodeFlags::None;
                node->parent->token.type  = TokenType::Identifier;

            } else {
                node->parent->flag = TokenNodeFlags::Bad;
            }

        } else if(buffer->data[index] == '"') {
            token.type = TokenType::Identifier;
        } else if(buffer->data[index] == '\'') {
            tokens_handle_cpp_style_comments();
        } else if(buffer->data[index] == '{') {

            // an open brace defined a new scope within the thing
            tree->current->next = node;
            node->token.type  = (enum TokenType)'{';

        } else if(buffer->data[index] == '}') {

            if(node->first->token.type != (enum TokenType)'{') {
                // @TODO: do something here i forgot what

            } else {

                Token token = {};
                tree->current->last = node;

                // @TODO: extract into function later
                {
                    token.type               = TokenType::Param;
                    token.line               = amount_of_newlines;
                    token.column             = index / amount_of_newlines;
                    token.byte_offset        = index;
                    token.value.string_value       = ToString8('}', 1);
                }

                if(is_token_node_nil(tree->current->last)) {

                    { // fill in the node
                        node->token; // do we want this as a pointer

                        node->flag                  = TokenNodeFlags::Child; // @TODO: no flags yet
#if 0
                        node->file_reference.reference_to_root_node = tree->root; // case: we are not the root node so we reference to it to find out in which file we are
#endif
                        node->previous = tree->current;
                        node->parent = node->previous->parent;

                        FillTokenNodeFieldWithNilNode(last);
                        FillTokenNodeFieldWithNilNode(first);
                        FillTokenNodeFieldWithNilNode(child);
                        FillTokenNodeFieldWithNilNode(next);
                    }

                } else {
                    //
                    // @TODO: the last node is not a  nil node and we should fill it up.
                    // this is kind of a weird scenario though. because when working whild children nods you should always have a next node.
                    //

                }

            }

        } else if(buffer->data[index] == '(') {

	  String8 expected_delimiters = ToString8(')' , 1); 

	  // @TODO: we should also have someting for legal delimiters and illegal delimiters.
	  // so that we can build a recursive tokenizer based on the fact that we get a delimiter or not


	  PushTokenChildren(tree, ')', expected_delimiters);


        } else if(buffer->data[index] == ')') {

	  if((u8)tree->current->first->token.value.string_value.data != (u8)')') { // @TODO: fix this ugly thing

	    // TODO: Logging
	  } else {

	    // TODO: Nothign
	  }

        } else if(buffer->data[index] == '[') {
        } else if(buffer->data[index] == ']') {

        } else if(buffer->data[index] == '#') {
            // @TODO HANDLE MODULE
        } else if(buffer->data[index] == ';' && (tree->current->flag & TokenNodeFlags::Child) != 0) {

        }

        else if(buffer->data[index] == ':') {

            // @TODO: typecheckiing

        } else {
            Log("Unrecognized token");
        }

        tree->root->flag = (TokenNodeFlags)(tree->root->flag | TokenNodeFlags::Eof);
        //- end fo file


    }


    return tree;
}



#endif /* LEXER_H */
