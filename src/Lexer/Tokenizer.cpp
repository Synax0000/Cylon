#include "Tokenizer.hpp"

#include <iostream>
#include <vector>
#include <string>

#include "Tokens.hpp"
#include "../Utils.hpp"
#include "../CommandLine/Output.hpp"

void HandleCharacter(char Character, int Line, int CurrentCharacterIndex, int TokenStartIndex, std::string &DataBuffer, std::vector<Token> &Tokens, std::string FilePath) {
    if (std::isalpha(Character) && (IsStringAlpha(DataBuffer) || DataBuffer == "")) {
        DataBuffer += Character;
        return;
    } else {
        if (IsStringAlpha(DataBuffer) && DataBuffer != "") {
            Tokens.push_back(Token(TokenType_Identifier, TokenVariant_Method, DataBuffer, TokenStartIndex, Line));
            DataBuffer = "";
        }
    }

    if ((std::isdigit(Character) || Character == '.') && (IsStringDigit(DataBuffer) || DataBuffer == "")) {
        DataBuffer += Character;
        return;
    } else {
        if (IsStringDigit(DataBuffer) && DataBuffer != "") {
            TokenVariant Varient = TokenVariant_Integer;

            if (HasCharacterInString(DataBuffer, '.') > 0) {
                if (HasCharacterInString(DataBuffer, '.') > 1) {
                    logsnippet(-1, FilePath, "[LINE: " + std::to_string(Line) + "] Number has too many decimal points '" + DataBuffer + "'", Line, TokenStartIndex);
                    exit(-1);
                }

                Varient = TokenVariant_Double;
            }

            if (Character == 'f') {
                Varient = TokenVariant_Float;
            }

            Tokens.push_back(Token(TokenType_Number, Varient, DataBuffer, TokenStartIndex, Line));
            DataBuffer = "";
        }
    }

    if (std::isalpha(Character) || std::isdigit(Character)) {
        return;
    }

    switch (Character) {
        case '=':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Assign, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '+':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Add, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '-':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Subtract, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '*':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Multiply, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '^':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Power, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '/':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Divide, std::string(1, Character), CurrentCharacterIndex, Line)); 
            return;

        case '>':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Inequality, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '<':
            Tokens.push_back(Token(TokenType_Operator, TokenVariant_Inequality, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case ':':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Colon, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '.':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Dot, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case ',':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Comma, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case ';':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_SemiColon, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '{':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Bracket, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '}':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Bracket, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '(':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Parentheses, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case ')':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_Parentheses, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '[':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_SquareBrackets, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case ']':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_SquareBrackets, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        case '\n':
            Tokens.push_back(Token(TokenType_Symbol, TokenVariant_NewLine, std::string(1, Character), CurrentCharacterIndex, Line));
            return;

        default:
            if (Character != ' ' && Character != '\t') {
                logsnippet(-1,  FilePath, "[LINE: " + std::to_string(Line) + "] Invaild Character '" + std::string(1, Character) + "'", Line, CurrentCharacterIndex);
                exit(-1);
            }
            return;
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

    return Tokens;
}