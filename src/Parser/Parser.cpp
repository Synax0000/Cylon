#include "Parser.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#include "ParserUtils.hpp"

#include "ParseExpression.hpp"
#include "ParseVariable.hpp"

#include "../Lexer/Tokens.hpp"

#include <iostream>
#include <vector>

AstNode* ParseStatement(std::string FilePath, std::vector<Token>& TokenList, std::vector<AstNode*>& NodeList, int& CurrentIndex, int& CurrentLine, int& CurrentCharacter) {
    if (CurrentIndex >= TokenList.size()) {
        return nullptr;
    }

    Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

    if (CurrentToken.Variant == TokenVariant_NewLine) {
        AdvanceToken(CurrentIndex, TokenList);
        return nullptr; 
    } else if (CurrentToken.Variant == TokenVariant_EOF) {
        return nullptr;
    }
    
    Token NextToken = PeekNext(TokenList, CurrentIndex);

    int ClonedIndex = CurrentIndex;

    std::vector<Token> ConsumedTokens = ConsumeTokenRange(TokenList, ClonedIndex, ";");
    bool StatementContainsAssignment = ContainsToken(ConsumedTokens, TokenVariant_Assign) || 
                                       ContainsToken(ConsumedTokens, TokenVariant_Add) ||
                                       ContainsToken(ConsumedTokens, TokenVariant_Subtract);

    if (ValidType(CurrentToken.Value, false) == true && (NextToken.Type == TokenType_Identifier || NextToken.Type == TokenType_Number)) {
        return ParseDeclaration(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    } else if (CurrentToken.Type == TokenType_Identifier && StatementContainsAssignment == true) {
        return ParseAssignment(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    } else {
        return ParseExpression(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    }

    return nullptr;
}

ProgramNode Parse(std::vector<Token> SourceTokens, std::string FileName, std::string FilePath) {
    ProgramNode Program = ProgramNode(AstNodeType_ProgramNode, -1, -1);
    
    Program.Tokens = SourceTokens;
    Program.FileName = FileName;
    Program.FilePath = FilePath;

    int CurrentIndex = 0;
    int CurrentLine = 0;
    int CurrentCharacter = 0;

    while (CurrentIndex < Program.Tokens.size()) {
        int PastIndex = CurrentIndex;

        AstNode* Statement = ParseStatement(FilePath, SourceTokens, Program.Nodes, CurrentIndex, CurrentLine, CurrentCharacter);
        if (Statement != nullptr) {
            Program.Body.push_back(Statement);
        }

        Token CurrentToken = GetCurrentToken(Program.Tokens, CurrentIndex);

        if (CurrentIndex == PastIndex || 
            CurrentToken.Variant == TokenVariant_SemiColon ||
            CurrentToken.Variant == TokenVariant_NewLine ||
            CurrentToken.Variant == TokenVariant_EOF
        ) {
            AdvanceToken(CurrentIndex, Program.Tokens);
        }
    }

    return Program;
}
