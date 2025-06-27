#include "Utils.hpp"

#include "Lexer/Tokens.hpp"
#include "Parser/DataTypes.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

bool IsStringAlpha(std::string Text) {
    for (char Character : Text) {
        if (!isalpha(Character)) {
            return false;
        }
    }

    return true;
}

bool IsStringDigit(std::string Text) {
    for (char Character : Text) {
        if (!isdigit(Character) && Character != '.') {
            return false;
        }
    }

    return true;
}

int HasCharacterInString(std::string Text, char TargetCharacter) {
    int CharactersFound = 0;

    for (char Character : Text) {
        if (Character == TargetCharacter) {
            CharactersFound += 1;
        }
    }

    return CharactersFound;
}

std::string OperatorTypeToString(TokenVariant Variant) {
    static const std::unordered_map<TokenVariant, std::string> OperatorMap = {
        { TokenVariant_Add, "Add" },
        { TokenVariant_Subtract, "Subtract" },
        { TokenVariant_Divide, "Divide" },
        { TokenVariant_Power, "Power" },
        { TokenVariant_Multiply, "Multiply" },
        { TokenVariant_Inequality, "Inequality" },
        { TokenVariant_Assign, "Assign" }
    };

    auto Iterator = OperatorMap.find(Variant);
    if (Iterator != OperatorMap.end()) {
        return Iterator->second;
    }

    return "Unknown Operation";
}

std::string NumberTypeToString(TokenVariant Variant) {
    static const std::unordered_map<TokenVariant, std::string> NumberMap = {
        { TokenVariant_Integer, "Integer" },
        { TokenVariant_Float, "Float" },
        { TokenVariant_Double, "Double" }
    };

    auto Iterator = NumberMap.find(Variant);
    if (Iterator != NumberMap.end()) {
        return Iterator->second;
    }

    return "Unknown Type";
}

std::string TokenTypeToString(TokenVariant Variant) {
    static const std::unordered_map<TokenVariant, std::string> TokenMap = {
        { TokenVariant_Method, "Method" },
        { TokenVariant_Integer, "Integer" },
        { TokenVariant_Float, "Float" },
        { TokenVariant_Double, "Double" },
        { TokenVariant_String, "String" },
        { TokenVariant_Character, "Character" },
        { TokenVariant_Add, "Add" },
        { TokenVariant_Subtract, "Subtract" },
        { TokenVariant_Divide, "Divide" },
        { TokenVariant_Multiply, "Multiply" },
        { TokenVariant_Power, "Power" },
        { TokenVariant_Assign, "Assign" },
        { TokenVariant_Inequality, "Inequality" },
        { TokenVariant_Bracket, "Bracket" },
        { TokenVariant_Parentheses, "Parentheses" },
        { TokenVariant_Dot, "Dot" },
        { TokenVariant_Colon, "Colon" },
        { TokenVariant_Comma, "Comma" },
        { TokenVariant_SemiColon, "Semicolon" },
        { TokenVariant_SquareBracket, "SquareBrackets" },
        { TokenVariant_NewLine, "Newline" }
    };

    auto Iterator = TokenMap.find(Variant);
    if (Iterator != TokenMap.end()) {
        return Iterator->second;
    }

    return "Unknown";
}

std::string GetLineSnippet(std::string FilePath, int Line) {
    std::ifstream File(FilePath);

    std::string CurrentLine;
    int LineIndex = 1;

    if (File.is_open()) {
        while (std::getline(File, CurrentLine)) {
            if (LineIndex == Line) {
                return CurrentLine;
            }

            LineIndex += 1;
        }
    }

    return "Unable to fetch code snippet for line " + std::to_string(Line);
}

DataTypeVariant StringToDataTypeVariant(std::string Value) {
    DataTypeVariant Variant;

    for (DataType Type : DataTypes) {
        if (Value == Type.Value) {
            Variant.Type = Type.Variants[0].Type;
            Variant.Value = Type.Value;

            return Variant;
        }

        for (DataTypeVariant CurrentVariant : Type.Variants) {
            if (Value == CurrentVariant.Value) {
                Variant.Type = CurrentVariant.Type;
                Variant.Value = CurrentVariant.Value;

                return Variant;
            }
        }
    }

    Variant.Value = "Invalid";

    return Variant;
}

TokenVariant StringToTokenOperation(std::string Value) {
    static const std::unordered_map<std::string, TokenVariant> OperatorMap = {
        { "=", TokenVariant_Assign },
        { "==", TokenVariant_AssignAssign },
        { "+", TokenVariant_Add },
        { "++", TokenVariant_Increment },
        { "+=", TokenVariant_AddAssign },
        { "-", TokenVariant_Subtract },
        { "--", TokenVariant_Decrement },
        { "-=", TokenVariant_SubtractAssign },
        { "*", TokenVariant_Multiply },
        { "*=", TokenVariant_MultiplyAssign },
        { "/", TokenVariant_Divide },
        { "/=", TokenVariant_DivideAssign },
        { "^", TokenVariant_Power },
        { "^=", TokenVariant_PowerAssign },
        { "!=", TokenVariant_InequalityAssign },
        { "<", TokenVariant_LessThan },
        { ">", TokenVariant_GreaterThan },
        { "<=", TokenVariant_LessThanAssign },
        { ">=", TokenVariant_GreaterThanAssign }
    };

    auto Iterator = OperatorMap.find(Value);
    if (Iterator != OperatorMap.end()) {
        return Iterator->second;
    }

    return TokenVariant_Invalid;
}

std::string TokenOperationToString(TokenVariant Variant) {
    static const std::unordered_map<TokenVariant, std::string> ReverseOperatorMap = {
        { TokenVariant_Assign, "=" },
        { TokenVariant_AssignAssign, "==" },
        { TokenVariant_Add, "+" },
        { TokenVariant_Increment, "++" },
        { TokenVariant_AddAssign, "+=" },
        { TokenVariant_Subtract, "-" },
        { TokenVariant_Decrement, "--" },
        { TokenVariant_SubtractAssign, "-=" },
        { TokenVariant_Multiply, "*" },
        { TokenVariant_MultiplyAssign, "*=" },
        { TokenVariant_Divide, "/" },
        { TokenVariant_DivideAssign, "/=" },
        { TokenVariant_Power, "^" },
        { TokenVariant_PowerAssign, "^=" },
        { TokenVariant_InequalityAssign, "!=" },
        { TokenVariant_Inequality, "!" },
        { TokenVariant_LessThan, "<" },
        { TokenVariant_GreaterThan, ">" },
        { TokenVariant_LessThanAssign, "<=" },
        { TokenVariant_GreaterThanAssign, ">=" }
    };

    auto Iterator = ReverseOperatorMap.find(Variant);
    if (Iterator != ReverseOperatorMap.end()) {
        return Iterator->second;
    }

    return "Invalid";
}

std::string BoolToYesNo(bool Value) {
    if (Value == true) {
        return "Yes";
    } else {
        return "No";
    }
}