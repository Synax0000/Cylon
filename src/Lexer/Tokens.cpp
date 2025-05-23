#include "Tokens.hpp"

#include <iostream>
#include <vector>

std::vector<Type> TokenMeanings = {
    Type(
        {
            TokenType_Identifier
        },
        "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_"
    ),

    Type(
        {
            TokenType_Number
        },
        "1234567890."
    ),
    
    Type( 
        {
            TokenType_Operator
        },
        "/*-+^"
    ),

    Type(
        {
            TokenType_String
        },
        "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890`~!@#$%^&*()_+|-=[]{},.<>/?;:'\""
    ),

    Type(
        {
            TokenType_Symbol
        },
        "[]{}<>:."
    ),
};
