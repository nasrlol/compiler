#ifndef LEXER_H
#define LEXER_H

typedef struct Token Token;
typedef struct TokenNode TokenNode;
typedef struct BufferProperties BufferProperties;
typedef struct TokenList TokenList;

#define DEBUG_STR8_DIFF(tree, arena, diff_size) debug_str8_diff(tree, arena, (diff_size), __FUNCTION__)

#define TokenNodeContainsFlag(node, flag)	  \
if(((node->flags) & (flag)) != (0))		\
return false

#define FillTokenNodeFieldWithNilNode(field)		    \
if(node->field == NULL || !is_token_node_nil(node)) \
node->field = &nil_token_node;

enum TokenType: u32
{
    TokenType_None = 0,
	
	TokenType_Double_Equal,
	TokenType_Not_Equal,
	TokenType_Greater_Equal,
	TokenType_Lesser_Equal,
	TokenType_Plus_Equal,
	TokenType_Minus_Equal,
	TokenType_Star_Equal,
	TokenType_Slash_Equal,
	TokenType_Right_Shift,
	TokenType_Left_Shift,
	TokenType_Double_Ampersand,
	TokenType_Double_Pipe,
	TokenType_Arrow,
	
	TokenType_Identifier,
	TokenType_Keyword,
	TokenType_Number,
	TokenType_Delimiter,
	
};

typedef enum Keyword : u32
{
	Keyword_None,
	Keyword_Struct,
	Keyword_While,
	Keyword_Return,
	Keyword_For,
	Keyword_Import,
	Keyword_Switch,
	Keyword_Case,
	Keyword_Defer,
	Keyword_Enum
} Keyword ;


struct Token
{
    enum TokenType type;
    u64            line;
	
    union
	{
		Keyword keyword;
		String8 value;
    };
	
};

struct TokenNode
{
    TokenNode *next;
    TokenNode *previous;
    Token *token;
    u64 buffer_index;
};

struct BufferProperties
{
    u64 newlines;
};

struct TokenList
{
    TokenNode *first;
    TokenNode *last;
	TokenNode *current;
    u64 index;
	u64 size;
};

global_variable String8 reserved_words[] =
{
	[Keyword_None]   = str8(""),
	[Keyword_Struct] = str8("struct"),
	[Keyword_While]  = str8("while"),
	[Keyword_Return] = str8("return"),
	[Keyword_For]    = str8("for"),
	[Keyword_Import] = str8("import"),
	[Keyword_Switch] = str8("switch"),
	[Keyword_Case]   = str8("case"),
	[Keyword_Defer]  = str8("defer"),
	[Keyword_Enum]   = str8("enum"),
};


ReadOnly global_variable String8 operators[] = {
	[TokenType_None]               = str8(""),
	[TokenType_Double_Equal]       = str8("=="),
	[TokenType_Not_Equal]          = str8("!="),
	[TokenType_Greater_Equal]      = str8("<="),
	[TokenType_Lesser_Equal]       = str8(">="),
	[TokenType_Plus_Equal]         = str8("+="),
	[TokenType_Minus_Equal]        = str8("-="),
	[TokenType_Star_Equal]         = str8("*="),
	[TokenType_Slash_Equal]        = str8("/="),
	[TokenType_Right_Shift]        = str8("<<"),
	[TokenType_Left_Shift]         = str8(">>"),
	[TokenType_Double_Ampersand]   = str8("&&"),
	[TokenType_Double_Pipe]        = str8("||"),
	[TokenType_Arrow]              = str8("->"),
};



ReadOnly global_variable Token nil_token = {};
ReadOnly global_variable TokenNode nil_token_node = {};
ReadOnly global_variable TokenList nil_token_list = {};

#endif // LEXER_H
