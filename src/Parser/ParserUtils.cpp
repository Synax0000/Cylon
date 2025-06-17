#include "ParserUtils.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

Token GetCurrentToken(std::vector<Token>& TokenList, int& CurrentIndex) {
    if (CurrentIndex >= TokenList.size() || CurrentIndex < 0) {
        log(-1, "Out of bounds index for 'GetCurrentToken', index: " + std::to_string(CurrentIndex));
        exit(-1);
    }

    return TokenList[CurrentIndex];
}

void AdvanceToken(int& CurrentIndex, std::vector<Token>& TokenList) {
    if (CurrentIndex + 1 < TokenList.size()) {
        CurrentIndex++;
    }
}

void UpdateCurrentLocation(std::vector<Token>& TokenList, int& CurrentIndex, int& CurrentLine, int& CurrentCharacter) {
    Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

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
            CurrentIndex = i + 1;
            return CollectedTokens;
        }

        CollectedTokens.push_back(CurrentToken);
    }

    return CollectedTokens;
}
