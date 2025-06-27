#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#ifndef PARSEVARIABLE_HPP
#define PARSEVARIABLE_HPP

AstNode* ParseDeclaration(
    std::string FilePath, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int &CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
);

AstNode* ParseAssignment(
    std::string FilePath, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int &CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
);

#endif