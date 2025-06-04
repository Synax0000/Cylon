#include "utils.hpp"

#include "Lexer/Tokens.hpp"

#include <iostream>
#include <string>
#include <fstream>

bool IsStringAlpha(std::string Text)
{
    for (char Character : Text)
    {
        if (!isalpha(Character))
        {
            return false;
        }
    }

    return true;
}

bool IsStringDigit(std::string Text)
{
    for (char Character : Text)
    {
        if (!isdigit(Character) && Character != '.')
        {
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
    switch (Variant) {
        case TokenVariant_Add:
            return "Add";

        case TokenVariant_Subtract:
            return "Subtract";

        case TokenVariant_Divide:
            return "Divide";

        case TokenVariant_Power:
            return "Power";

        case TokenVariant_Multiply:
            return "Multiply";

        case TokenVariant_Inequality:
            return "Inequality";

        case TokenVariant_Assign:
            return "Assign";

        default:
            return "Unknown Operation";
    }
}

std::string NumberTypeToString(TokenVariant Variant) {
    switch (Variant) {
        case TokenVariant_Integer:
            return "Integer";

        case TokenVariant_Float:
            return "Float";

        case TokenVariant_Double:
            return "Double";

        default:
            return "Unknown Type";
    }
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
