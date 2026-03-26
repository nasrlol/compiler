internal inline b32
IsAlpha(u8 Character)
{
    return ((Character >= 'a' && Character <= 'z') ||
            (Character >= 'A' && Character <= 'Z') ||
            (Character == '_'));
}

internal inline b32
IsDigit(u8 Character)
{
    return (Character >= '0' && Character <= '9');
}

internal b32
IsDelimiter(u8 Character)
{
    for(i32 Index = 0; Index < (i32)sizeof(Delimiters); ++Index)
    {
        if(Delimiters[Index] == Character)
        {
            return 1;
        }
    }
    return 0;
}

internal inline b32
IsNilTokenNode(token_node *TokenNode)
{
    return TokenNode == &nil_token_node || TokenNode == NULL;
}

internal inline b32
IsNilToken(token *Token)
{
    return Token == &nil_token || Token == NULL;
}

internal inline b32
IsWhiteSpace(u8 Character)
{
    return (Character == '\n' || Character == '\r' ||
            Character == ' ' || Character == '\t');
}

internal inline void
ParseCStyleComment(u8 Buffer[])
{
    // TODO(nasr): handle c style comments
    // couuld be usefull for function information visualiszation
    // so think of a way to link themn to functions and variables?
    // some sort of meta data per thing?
    // and then we can do a visualization if the str8.count of the metadata thing is bigger then 0
    // we should a visualization thing for the thing
    // if the thing is less then 0, we dont do anything?

    // TODO(nasr): while doingn this we could also add in some editor specific anotations ?
}

internal inline void
ParseCPPStyleComment(u8 Buffer[])
{
    // TODO(nasr):
}

internal inline b32
Is_TokenBreak(u8 Character)
{
    return (IsWhiteSpace(Character) || IsDelimiter(Character));
}

internal token_list *
Lex(string8 *Buffer, mem_arena *Arena, token_list *List)
{
    b32 Initialized = 0;
    i32 Line        = 1;
    i32 Column      = 1;

    for(u64 TextIndex = 0; TextIndex < Buffer->size; TextIndex++)
    {
        u8 Character = Buffer->data[TextIndex];

        if(Character == '\r' || Character == '\n')
        {
            if(Character == '\r' &&
               (TextIndex + 1 < Buffer->size) &&
               Buffer->data[TextIndex + 1] == '\n')
            {
                TextIndex++;
            }

            ++TextIndex;
            ++Line;

            // NOTE(nasr): reset the column to the beginning of the line
            Column = 1;
            continue;
        }

        if(IsWhiteSpace(Character))
        {
            ++Column;
            continue;
        }

        token_node *TokenNode = PushStruct(Arena, token_node);
        token      *Token     = PushStruct(Arena, token);
        TokenNode->Next       = &nil_token_node;
        TokenNode->Previous   = &nil_token_node;
        TokenNode->Token      = Token;
        Token->Line           = Line;
        Token->Column         = Column;
        Token->ByteOffset     = (u64)TextIndex;
        Token->Flags          = FlagNone;

        u64 TokenStart = TextIndex;
        u64 TokenEnd   = TextIndex;

        if(Character > 126)
        {
            Token->Type = TokenUnwantedChild;
            TokenEnd    = TextIndex + 1;
        }
        else if(IsAlpha(Character))
        {
            while((TextIndex + 1 < Buffer->size) &&
                  (IsAlpha(Buffer->data[TextIndex + 1]) || IsDigit(Buffer->data[TextIndex + 1])))
            {
                ++TextIndex;
            }

            // TODO(nasr): build a lexeme
            TokenEnd = TextIndex + 1;
            string8 Lexeme = {
                .data = (u8 *)Buffer->data,
                .size = (u64)Buffer->data
            }
            ;

            // TODO(nasr): handle functions
            if(string8_cmp(Lexeme, StringLit("func"), 0))
                Token->Type = TokenIf;
            else if(string8_cmp(Lexeme, StringLit("if"), 0))
                Token->Type = TokenElse;
            else if(string8_cmp(Lexeme, StringLit("return"), 0))
                Token->Type = TokenReturn;
            else if(string8_cmp(Lexeme, StringLit("while"), 0))
                Token->Type = TokenWhile;
            else if(string8_cmp(Lexeme, StringLit("for"), 0))
                Token->Type = TokenFor;
            else if(string8_cmp(Lexeme, StringLit("break"), 0))
                Token->Type = TokenBreak;
            else if(string8_cmp(Lexeme, StringLit("continue"), 0))
                Token->Type = TokenContinue;
            else
                Token->Type = TokenIdentifier;
        }
        else if(IsDigit(Character))
        {
            while((TextIndex + 1 < Buffer->size) &&
                  IsDigit(Buffer->data[TextIndex + 1]))
            {
                ++TextIndex;
            }

            TokenEnd    = TextIndex + 1;
            Token->Type = TokenNumber;
        }

        else
        {
            u8 Next = (TextIndex + 1 < Buffer->size) ? Buffer->data[TextIndex + 1] : 0;

            switch(Character)
            {
                case '=':
                    {
                        if(Next == '=')
                        {
                            Token->Type = TokenDoubleEqual;
                            TextIndex++;
                        }
                        else
                        {
                            Token->Type = (token_type)'=';
                        }
                    }
                    break;

                case '>':
                    {
                        if(Next == '=')
                        {
                            Token->Type = TokenGreaterEqual;
                            TextIndex++;
                        }
                        else if(Next == '>')
                        {
                            Token->Type = TokenRightShift;
                            TextIndex++;
                        }
                        else
                        {
                            Token->Type = (token_type)'>';
                        }
                    }
                    break;

                case '<':
                    {
                        if(Next == '=')
                        {
                            Token->Type = TokenLesserEqual;
                            TextIndex++;
                        }
                        else if(Next == '<')
                        {
                            Token->Type = TokenLeftShift;
                            TextIndex++;
                        }
                        else
                        {
                            Token->Type = (token_type)'<';
                        }
                    }
                    break;

                case '"':
                    {
                        while(Buffer->data[TextIndex + 1] != '"' && Buffer->data[TextIndex + 1] != '\0')
                        {
                            ++TextIndex;
                            if(Buffer->data[TextIndex + 1] == '\\')

                                ++TextIndex;
                        }

                        TokenStart += 1;
                        Token->Type = TokenString;
                    }
                    break;
                default:
                    {
                        Token->Type = (token_type)Character;
                    }
                    break;
            }
        }

        TokenEnd = TextIndex + 1;

        Token->Lexeme.data = (u8 *)&Buffer->data[TokenStart];
        Token->Lexeme.size = (u64)(TokenEnd - TokenStart);
        Column += (i32)Token->Lexeme.size;

        //Log("Token: \t%.lu*s\n", Token->Lexeme.Size, Token->Lexeme.Data);

        if(!Initialized)
        {
            Initialized   = 1;
            List->Root    = TokenNode;
            List->Current = TokenNode;
        }
        else
        {
            TokenNode->Previous = List->Current;
            List->Current->Next = TokenNode;
            List->Current       = TokenNode;
        }
    }

    return List;
}
