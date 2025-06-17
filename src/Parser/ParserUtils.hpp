#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#ifndef PARSERUTILS_HPP
#define PARSERUTILS_HPP


Token GetCurrentToken(std::vector<Token>& TokenList, int& CurrentIndex);
void AdvanceToken(int& CurrentIndex, std::vector<Token>& TokenList);
void UpdateCurrentLocation(std::vector<Token>& TokenList, int& CurrentIndex, int& CurrentLine,int& CurrentCharacter);
bool MatchToken(std::vector<Token>& TokenList, int &CurrentLine, std::string ExpectedMatch);
std::vector<Token> ConsumeTokenRange(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch);

#endif