#ifndef LEXER_H
#define LEXER_H

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
    s32 line;
    s32 column;
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
    string8     lexeme;
    token_type  type;
    token_flags flags;
    u64         byteOffset;
    s32         column;
    s32         line;

    string8 MetaData;
};

typedef struct token_node token_node;
struct token_node
{
    token_node *next;
    token_node *previous;
    token_node *child;
    token_node *parent;
    token      *token;
};

typedef struct token_tree token_tree;
struct token_tree
{
    token_node *root;
    token_node *current;
};

typedef struct lexer lexer;
struct lexer
{
    string8 *text;
    u8 *undefinedTokens;
};

global_variable const
u8 delimiters[] =
{
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    ';',
};

read_only global_variable
token nil_token =
{
    .lexeme     = {NULL, 0},
    .type       = TokenUndefined,
    .flags      = FlagNone,
    .byteOffset = 0,
    .column     = 0,
    .line       = 0,
};

read_only global_variable
token_node nil_token_node =
{
    .next     = &nil_token_node,
    .previous = &nil_token_node,
    .token    = NULL,
};

#endif /* LEXER_H */
