#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#ifndef PARSEEXPRESSION_HPP
#define PARSEEXPRESSION_HPP

AstNode* ParseTerm(
    std::string FilePath, 
    std::vector<AstNode*>& Body, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int& CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
);

AstNode* ParseFactor(
    std::string FilePath, 
    std::vector<AstNode*>& Body, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int& CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
);

AstNode* ParseExpression(
    std::string FilePath, 
    std::vector<AstNode*>& Body, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int& CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
);

#endif