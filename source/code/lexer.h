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
    TokenType_Illegal = 256,
	TokenType_Identifier,
	TokenType_Keyword,
	TokenType_Number,
	TokenType_Delimiter,
	TokenType_Double_Equal,
	TokenType_Greater_Equal,
	TokenType_Lesser_Equal,
	TokenType_Param,
	TokenType_Right_Shift,
	TokenType_Left_Shift,
	TokenType_Asterisk,
	
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

ReadOnly global_variable Token nil_token = {};
ReadOnly global_variable TokenNode nil_token_node = {};
ReadOnly global_variable TokenList nil_token_list = {};

#endif // LEXER_H
