#include "utils.hpp"

#include <iostream>
#include <string>

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