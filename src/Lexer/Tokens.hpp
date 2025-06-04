#include <iostream>
#include <string>
#include <vector>

#ifndef TOKENS_HPP
#define TOKENS_HPP

typedef enum TokenType {
    TokenType_Identifier,
    TokenType_Number,
    TokenType_String,
    TokenType_Operator,
    TokenType_Symbol
} TokenType;

typedef enum TokenVariant {
    // TokenType_Identifier
    TokenVariant_Method,

    // TokenType_Number
    TokenVariant_Integer,
    TokenVariant_Float,
    TokenVariant_Double,

    // TokenType_String
    TokenVariant_String,
    TokenVariant_Character,

    // TokenType_Operator
    TokenVariant_Add,
    TokenVariant_Subtract,
    TokenVariant_Divide,
    TokenVariant_Multiply,
    TokenVariant_Power,
    TokenVariant_Assign,
    TokenVariant_Inequality, 

    // TokenType_Symbol
    TokenVariant_Bracket,
    TokenVariant_Parentheses,
    TokenVariant_Dot,
    TokenVariant_Colon,
    TokenVariant_Comma,
    TokenVariant_SemiColon,
    TokenVariant_SquareBrackets,
    TokenVariant_NewLine,
} TokenVariant;

typedef struct Token {
    TokenType Type;
    TokenVariant Variant;
    std::string Value;
    int CharacterIndex;
    
    Token(TokenType TypeValue,TokenVariant TokenVariantValue, std::string StringValue, int CharacterIndexValue) {
        Type = TypeValue;
        Variant = TokenVariantValue;
        Value = StringValue;
        CharacterIndex = CharacterIndexValue;
    }
} Token;

#endif