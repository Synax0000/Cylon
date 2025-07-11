#include <iostream>
#include <string>
#include <vector>

#ifndef TOKENS_HPP
#define TOKENS_HPP

typedef enum cTokenType { // "c" is placed there to avoid windows conflicts 
    TokenType_Identifier,
    TokenType_Number,
    TokenType_String,
    TokenType_Operator,
    TokenType_Symbol,
    TokenType_Special,
} cTokenType;

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
    TokenVariant_LessThan,
    TokenVariant_GreaterThan,

    TokenVariant_AddAssign,
    TokenVariant_SubtractAssign,
    TokenVariant_DivideAssign,
    TokenVariant_MultiplyAssign,
    TokenVariant_PowerAssign,
    TokenVariant_AssignAssign,
    TokenVariant_LessThanAssign,
    TokenVariant_GreaterThanAssign,
    TokenVariant_InequalityAssign, 

    TokenVariant_Increment,
    TokenVariant_Decrement,

    // TokenType_Symbol
    TokenVariant_Bracket,
    TokenVariant_Parentheses,
    TokenVariant_Dot,
    TokenVariant_Colon,
    TokenVariant_Comma,
    TokenVariant_SemiColon,
    TokenVariant_SquareBracket,
    TokenVariant_NewLine,

    TokenVariant_EOF,
    TokenVariant_Invalid 
} TokenVariant;

typedef struct Token {
    cTokenType Type;
    TokenVariant Variant;
    std::string Value;
    
    int CharacterIndex;
    int LineIndex;
    
    Token(cTokenType TypeValue,TokenVariant TokenVariantValue, std::string StringValue, int CharacterIndexValue, int LineIndexValue) {
        Type = TypeValue;
        Variant = TokenVariantValue;
        Value = StringValue;
        CharacterIndex = CharacterIndexValue;
        LineIndex = LineIndexValue;
    }
} Token;

#endif