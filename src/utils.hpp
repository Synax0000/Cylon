#include <iostream>
#include <string>

#include "Lexer/Tokens.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP

bool IsStringAlpha(std::string Text);
bool IsStringDigit(std::string Text);
int HasCharacterInString(std::string Text, char TargetCharacter);
std::string OperatorTypeToString(TokenVariant Variant);
std::string NumberTypeToString(TokenVariant Variant);
std::string GetLineSnippet(std::string FilePath, int Line);

#endif