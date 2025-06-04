#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"

#include <iostream>
#include <vector>

#ifndef PARSER_HPP
#define PARSER_HPP

ProgramNode Parse(std::vector<Token> SourceTokens, std::string FileName, std::string FilePath);

#endif