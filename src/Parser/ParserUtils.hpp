#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#ifndef PARSERUTILS_HPP
#define PARSERUTILS_HPP

Token GetCurrentToken(std::vector<Token>& TokenList, int& CurrentIndex);
void AdvanceToken(int& CurrentIndex, std::vector<Token>& TokenList);
void Advance(int& CurrentIndex, std::vector<Token>& TokenList, int Offset);
void UpdateCurrentLocation(std::vector<Token>& TokenList, int& CurrentIndex, int& CurrentLine,int& CurrentCharacter);
bool MatchToken(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch);
std::vector<Token> ConsumeTokenRange(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch);
std::vector<Token> ConsumeFullTokenRange(std::vector<Token>& TokenList, int &CurrentIndex, std::string ExpectedMatch, bool ConsumeLastToken);
Token PeekNext(std::vector<Token>& TokenList, int& CurrentIndex);
Token Peek(std::vector<Token>& TokenList, int& CurrentIndex, int Offset);
bool ValidType(std::string Value, bool ExcludeModifiers);
bool ContainsToken(std::vector<Token>& TokenList, TokenVariant Variant);
int FindIndexOfToken(std::vector<Token>& TokenList, int &CurrentIndex, TokenVariant Variant);
Token PeekBehind(std::vector<Token>& TokenList, int& CurrentIndex);
Token Backtrack(std::vector<Token>& TokenList, int& CurrentIndex, int Offset);

#endif