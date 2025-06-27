#include "ParserUtils.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#include "DataTypes.hpp"

Token GetCurrentToken(std::vector<Token>& TokenList, int& CurrentIndex) {
    if (CurrentIndex >= TokenList.size() || CurrentIndex < 0) {
        return Token(TokenType_Special, TokenVariant_EOF, "EOF", TokenList[TokenList.size() - 1].CharacterIndex, TokenList[TokenList.size() - 1].LineIndex);
    }

    return TokenList[CurrentIndex];
}

void AdvanceToken(int& CurrentIndex, std::vector<Token>& TokenList) {
    if (CurrentIndex < TokenList.size()) {
        CurrentIndex++;
    } else {
        CurrentIndex = TokenList.size();
    }
}

void Advance(int& CurrentIndex, std::vector<Token>& TokenList, int Offset) {
    if (CurrentIndex < TokenList.size()) {
        CurrentIndex += Offset;
    } else {
        CurrentIndex = TokenList.size();
    }
}

void UpdateCurrentLocation(std::vector<Token>& TokenList, int& CurrentIndex, int& CurrentLine, int& CurrentCharacter) {
    Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

    if (CurrentToken.Variant == TokenVariant_EOF) {
        return;
    }

    CurrentLine = CurrentToken.LineIndex;
    CurrentCharacter = CurrentToken.CharacterIndex;
}

bool MatchToken(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch) {
    for (int i=CurrentIndex; i<TokenList.size(); i++) {
        if (GetCurrentToken(TokenList,i).Value == ExpectedMatch) {
            return true;
        }
    }

    return false;
}

std::vector<Token> ConsumeTokenRange(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch) {
    std::vector<Token> CollectedTokens;

    if (MatchToken(TokenList, CurrentIndex, ExpectedMatch) == false) {
        return CollectedTokens;
    }

    for (int i = CurrentIndex + 1; i < TokenList.size(); i++) {
        Token CurrentToken = GetCurrentToken(TokenList, i);

        if (CurrentToken.Value == ExpectedMatch) {
            if (i+1 <= TokenList.size()) {
                CurrentIndex = i + 1;
            }

            return CollectedTokens;
        }

        if (CurrentToken.Variant != TokenVariant_NewLine && CurrentToken.Variant != TokenVariant_EOF) {
            CollectedTokens.push_back(CurrentToken);
        }
    }

    return CollectedTokens;
}

std::vector<Token> ConsumeFullTokenRange(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch, bool ConsumeLastToken) {
    std::vector<Token> CollectedTokens;

    if (MatchToken(TokenList, CurrentIndex, ExpectedMatch) == false) {
        return CollectedTokens;
    }

    for (int i = CurrentIndex; i < TokenList.size(); i++) {
        Token CurrentToken = GetCurrentToken(TokenList, i);

        if (CurrentToken.Value == ExpectedMatch) {
            if (i+1 <= TokenList.size()) {
                CurrentIndex = i + 1;
            }

            if (ConsumeLastToken) {
                CollectedTokens.push_back(CurrentToken);
            }

            return CollectedTokens;
        }

        if (CurrentToken.Variant != TokenVariant_NewLine && CurrentToken.Variant != TokenVariant_EOF) {
            CollectedTokens.push_back(CurrentToken);
        }
    }

    return CollectedTokens;
}

Token PeekNext(std::vector<Token>& TokenList, int& CurrentIndex) {
    if (CurrentIndex >= TokenList.size() || CurrentIndex < 0) {
        log(-1, "Out of bounds index for 'PeekNext', index: " + std::to_string(CurrentIndex));
        exit(-1);
    }    

    return TokenList[CurrentIndex + 1];
}

Token Peek(std::vector<Token>& TokenList, int& CurrentIndex, int Offset) {
        if (CurrentIndex >= TokenList.size() || CurrentIndex < 0) {
        log(-1, "Out of bounds index for 'Peek', index: " + std::to_string(CurrentIndex));
        exit(-1);
    }    

    return TokenList[CurrentIndex + Offset];
}

bool ValidType(std::string Value, bool ExcludeModifiers) {
    for (DataType Data : DataTypes) {
        if (ExcludeModifiers == true) {
            if (Data.Modifier == true) {
                continue;
            }
        }

        for (DataTypeVariant Variant : Data.Variants) {
            if (Value == Variant.Value) {
                return true;
            }
        }

        if (Value == Data.Value) {
            return true;
        }
    }

    return false;
}

bool ContainsToken(std::vector<Token>& TokenList, TokenVariant Variant) {
    for (Token CurrentToken : TokenList) {
        if (CurrentToken.Variant == Variant) {
            return true;
        }
    }

    return false;
}

int FindIndexOfToken(std::vector<Token>& TokenList, int &CurrentIndex, TokenVariant Variant) {
    for (int i=CurrentIndex; i<TokenList.size(); i++) {
        if (TokenList[i].Variant == Variant) {
            return i;
        }
    }

    return -1;
}
 
Token PeekBehind(std::vector<Token>& TokenList, int& CurrentIndex) {
    if (CurrentIndex >= TokenList.size() || CurrentIndex < 0) {
        log(-1, "Out of bounds index for 'PeekNext', index: " + std::to_string(CurrentIndex));
        exit(-1);
    }    

    return TokenList[CurrentIndex - 1];
}

Token Backtrack(std::vector<Token>& TokenList, int& CurrentIndex, int Offset) {
    if (CurrentIndex >= TokenList.size() || CurrentIndex < 0) {
        log(-1, "Out of bounds index for 'Peek', index: " + std::to_string(CurrentIndex));
        exit(-1);
    }    

    return TokenList[CurrentIndex - Offset];
}