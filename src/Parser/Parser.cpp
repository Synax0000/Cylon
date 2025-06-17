#include "Parser.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#include "ParserUtils.hpp"
#include "ParseExpression.hpp"

#include <iostream>
#include <vector>

ProgramNode Parse(std::vector<Token> SourceTokens, std::string FileName, std::string FilePath) {
    ProgramNode Program = ProgramNode(AstNodeType_ProgramNode, -1, -1);
    
    Program.Tokens = SourceTokens;
    Program.FileName = FileName;
    Program.FilePath = FilePath;

    int CurrentIndex = 0;
    int CurrentLine = 0;
    int CurrentCharacter = 0;

    AstNode* Node = ParseExpression(FilePath, Program.Body, Program.Nodes, SourceTokens, CurrentIndex, CurrentLine, CurrentCharacter);
    Program.Body.push_back(Node);
    
    return Program;
}
