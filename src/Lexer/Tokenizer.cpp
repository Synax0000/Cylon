#include "Tokenizer.hpp"

#include <iostream>
#include <vector>
#include <string>

#include <unordered_map>

#include "Tokens.hpp"
#include "../Utils.hpp"
#include "../CommandLine/Output.hpp"

const std::unordered_map<char, std::pair<cTokenType, TokenVariant>> SingleCharacterTokens = {
    { '=', { TokenType_Operator, TokenVariant_Assign } },
    { '+', { TokenType_Operator, TokenVariant_Add } },
    { '-', { TokenType_Operator, TokenVariant_Subtract } },
    { '*', { TokenType_Operator, TokenVariant_Multiply } },
    { '^', { TokenType_Operator, TokenVariant_Power } },
    { '/', { TokenType_Operator, TokenVariant_Divide } },
    { '>', { TokenType_Operator, TokenVariant_GreaterThan } },
    { '<', { TokenType_Operator, TokenVariant_LessThan } },
    { ':', { TokenType_Symbol, TokenVariant_Colon } },
    { '!', { TokenType_Symbol, TokenVariant_Inequality } },
    { '.', { TokenType_Symbol, TokenVariant_Dot } },
    { ',', { TokenType_Symbol, TokenVariant_Comma } },
    { ';', { TokenType_Symbol, TokenVariant_SemiColon } },
    { '{', { TokenType_Symbol, TokenVariant_Bracket } },
    { '}', { TokenType_Symbol, TokenVariant_Bracket } },
    { '(', { TokenType_Symbol, TokenVariant_Parentheses } },
    { ')', { TokenType_Symbol, TokenVariant_Parentheses } },
    { '[', { TokenType_Symbol, TokenVariant_SquareBracket } },
    { ']', { TokenType_Symbol, TokenVariant_SquareBracket } },
    { '\n', { TokenType_Symbol, TokenVariant_NewLine } }
};

void HandleCharacter(char Character, int Line, int CurrentCharacterIndex, int& TokenStartIndex, std::string& DataBuffer, std::vector<Token>& Tokens, std::string FilePath) {
    if (std::isalnum(Character)) {
        if (DataBuffer.empty() || std::isalnum(DataBuffer.back())) {
            DataBuffer += Character;
            return;
        } else {
            if (std::isalpha(DataBuffer.back())) {
                Tokens.push_back(Token(TokenType_Identifier, TokenVariant_Method, DataBuffer, TokenStartIndex, Line));
            } else {
                TokenVariant Variant = HasCharacterInString(DataBuffer, '.') > 0 ? TokenVariant_Double : TokenVariant_Integer;
                Tokens.push_back(Token(TokenType_Number, Variant, DataBuffer, TokenStartIndex, Line));
            }

            DataBuffer.clear();
            TokenStartIndex = CurrentCharacterIndex;
            DataBuffer += Character;
            return;
        }
    } else {
        if (!DataBuffer.empty()) {
            if (std::isalpha(DataBuffer.front())) {
                Tokens.push_back(Token(TokenType_Identifier, TokenVariant_Method, DataBuffer, TokenStartIndex, Line));
            } else {
                TokenVariant Variant = HasCharacterInString(DataBuffer, '.') > 0 ? TokenVariant_Double : TokenVariant_Integer;
                Tokens.push_back(Token(TokenType_Number, Variant, DataBuffer, TokenStartIndex, Line));
            }

            DataBuffer.clear();
        }

        if (Character == ' ' || Character == '\t') {
            TokenStartIndex = CurrentCharacterIndex + 1;
            return;
        }

        auto Iterator = SingleCharacterTokens.find(Character);
        if (Iterator != SingleCharacterTokens.end()) {
            Tokens.push_back(Token(Iterator->second.first, Iterator->second.second, std::string(1, Character), CurrentCharacterIndex, Line));
        } else {
            logsnippet(-1, FilePath, "[LINE: " + std::to_string(Line) + "] Invalid Character '" + std::string(1, Character) + "'", Line, CurrentCharacterIndex);
            exit(-1);
        }

        TokenStartIndex = CurrentCharacterIndex + 1;
    }
}

std::vector<Token> Tokenize(std::string Source, std::string FilePath) {
    std::vector<Token> Tokens = {};

    std::string DataBuffer = "";

    int Line = 1;
    int CharacterIndex = 0;
    int TokenStartIndex = 0;

    for (size_t Index = 0; Index < Source.size(); Index++) {
        char Character = Source[Index];

        if (Character == '\n') {
            Line++;
            CharacterIndex = 0;
            TokenStartIndex = 0;
        }

        if (DataBuffer.empty()) {
            TokenStartIndex = CharacterIndex;
        }

        size_t PreviousSize = Tokens.size();
        HandleCharacter(Character, Line, CharacterIndex, TokenStartIndex, DataBuffer, Tokens, FilePath);

        bool Reprocess = Tokens.size() > PreviousSize && std::isalnum(Character);
        if (Reprocess) {
            Index--;
        }

        CharacterIndex++;
    }

    if (!DataBuffer.empty()) {
        HandleCharacter(' ', Line, CharacterIndex, TokenStartIndex, DataBuffer, Tokens, FilePath);
    }

    Tokens.push_back(Token(TokenType_Symbol, TokenVariant_EOF, "EOF", CharacterIndex, Line));

    return Tokens;
}