#ifndef FAJR_LEXER_H
#define FAJR_LEXER_H

typedef enum token_type token_type;
enum token_type
{
    TokenUndefined = 256,
    TokenIdentifier,
    TokenIdentifierAssignmentValue,
    TokenValue,
    TokenString,
    TokenNumber,
    TokenDoubleEqual,
    TokenGreaterEqual,
    TokenLesserEqual,
    TokenParam,
    TokenFunc,
    TokenReturn,
    TokenIf,
    TokenElse,
    TokenFor,
    TokenWhile,
    TokenBreak,
    TokenContinue,
    TokenExpression,
    TokenFuncBody,
    TokenUnwantedChild,
    TokenNewLine,
    TokenRightShift,
    TokenLeftShift,
    TokenStar,
};

typedef struct Tokenizer Tokenizer;
struct Tokenizer
{
    i32 Line;
    i32 Column;
};

typedef enum token_flags token_flags;
enum token_flags
{
    FlagNone       = (0),
    FlagConstant   = (1 << 0),
    FlagGlobal     = (1 << 1),
    FlagsValue     = (1 << 2),
    FlagDefinition = (1 << 3),
    FlagComparison = (1 << 4),
    FlagDeprecated = (1 << 5),
    FlagDirty      = (1 << 6),
};

typedef struct token token;
struct token
{
    string8     Lexeme;
    token_type  Type;
    token_flags Flags;
    u64         ByteOffset;
    i32         Column;
    i32         Line;

    string8 MetaData;
};

typedef struct token_node token_node;
struct token_node
{
    token_node *Next;
    token_node *Previous;
    token      *Token;
};

typedef struct token_list token_list;
struct token_list
{
    token_node *Root;
    token_node *Current;
};

typedef struct lexer lexer;
struct lexer
{
    u8 *Text;
    u64 TextCount;
    u8 *EndOfFile;
    u8 *UndefinedTokens;
};

global_variable const u8 Delimiters[] =
{
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    ';',
};

read_only global_variable token nil_token =
{
    .Lexeme     = {NULL, 0},
    .Type       = TokenUndefined,
    .Flags      = FlagNone,
    .ByteOffset = 0,
    .Column     = 0,
    .Line       = 0,
};

read_only global_variable token_node nil_token_node =
{
    .Next     = &nil_token_node,
    .Previous = &nil_token_node,
    .Token    = NULL,
};

#endif // FAJR_LEXER_H
