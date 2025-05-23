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
    TokenVariant_Keyword,
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
    TokenVarient_Equal,

    // TokenType_Symbol
    TokenVariant_Bracket,
    TokenVariant_Parentheses,
    TokenVariant_Inequality,
    TokenVariant_Dot,
    TokenVariant_Colon,
} TokenVariant;

typedef struct Token {
    TokenType Type;
    TokenVariant Variant;
    std::string Value;
    
    Token(TokenType TypeValue,TokenVariant TokenVariantValue, std::string StringValue) {
        Type = TypeValue;
        Variant = TokenVariantValue;
        Value = StringValue;
    }
} Token;

typedef struct Type {
    std::vector<TokenType> TargetTypes;
    std::string TargetCharacters;

    Type(std::vector<TokenType> TargetTypesValue, std::string TargetCharactersValue) {
        TargetTypes = TargetTypesValue;
        TargetCharacters = TargetCharactersValue;
    }
} Type;

extern std::vector<Type> TokenMeanings;

#endif