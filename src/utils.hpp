#include <iostream>
#include <string>

#include "Lexer/Tokens.hpp"
#include "Parser/DataTypes.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP

bool IsStringAlpha(std::string Text);
bool IsStringDigit(std::string Text);
int HasCharacterInString(std::string Text, char TargetCharacter);
std::string OperatorTypeToString(TokenVariant Variant);
std::string TokenTypeToString(TokenVariant Variant);
std::string NumberTypeToString(TokenVariant Variant);
std::string GetLineSnippet(std::string FilePath, int Line);
DataTypeVariant StringToDataTypeVariant(std::string Value);
TokenVariant StringToTokenOperation(std::string Value);
std::string TokenOperationToString(TokenVariant Variant);
std::string BoolToYesNo(bool Value);

#endif