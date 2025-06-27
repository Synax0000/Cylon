#include "ParseExpression.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#include "ParserUtils.hpp"

AstNode* ParseFactor(
    std::string FilePath,
    std::vector<AstNode*>& NodeList,
    std::vector<Token>& TokenList,
    int& CurrentIndex,
    int& CurrentLine,
    int& CurrentCharacter
) {
    if (CurrentIndex >= TokenList.size()) {
        return nullptr;
    }

    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);
    
    AstNode* LeftNode = nullptr;

    if (CurrentToken.Type == TokenType_Number) {
        LeftNode = new NumberNode(AstNodeType_NumberNode, CurrentToken.Variant, CurrentToken.Value, CurrentLine, CurrentCharacter);
        NodeList.push_back(LeftNode);

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    } else if (CurrentToken.Type == TokenType_Identifier) {
        LeftNode = new VariableNode(AstNodeType_VariableNode, CurrentToken.Value, CurrentLine, CurrentCharacter);
        NodeList.push_back(LeftNode);

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    } else if (CurrentToken.Variant == TokenVariant_Parentheses && CurrentToken.Value == "(") {
        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* InnerNode = ParseExpression(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        if (CurrentIndex >= TokenList.size() || MatchToken(TokenList, CurrentIndex, ")") == false) {
            logsnippet(-1, FilePath, "Expected closing parenthesis", CurrentLine, CurrentCharacter);
            exit(-1);
        }

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* GroupedNode = new GroupNode(AstNodeType_GroupNode, CurrentToken.LineIndex, CurrentToken.CharacterIndex);

        static_cast<GroupNode*>(GroupedNode)->Members.push_back(InnerNode);
        NodeList.push_back(GroupedNode);

        LeftNode = GroupedNode;

        Token NextToken = GetCurrentToken(TokenList, CurrentIndex);

        if (NextToken.Type == TokenType_Number || NextToken.Variant == TokenVariant_Parentheses && NextToken.Value == "(") {
            logsnippet(-1, FilePath, "Expected operator between expressions", CurrentLine, CurrentCharacter);
            exit(-1);
        }
    } else {
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
        logsnippet(-1, FilePath, "Invaild expression", CurrentLine, CurrentCharacter);
        exit(-1);
    }

    while (CurrentIndex < TokenList.size() && GetCurrentToken(TokenList, CurrentIndex).Variant == TokenVariant_Power) {
        int OperatorLine = GetCurrentToken(TokenList, CurrentIndex).LineIndex;
        int OperatorCharacter = GetCurrentToken(TokenList, CurrentIndex).CharacterIndex;

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* RightNode = ParseFactor(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
        LeftNode = new OperatorNode(AstNodeType_OperatorNode, LeftNode, RightNode, TokenVariant_Power, OperatorLine, OperatorCharacter);
    }

    return LeftNode;
}


AstNode* ParseTerm(
    std::string FilePath,
    std::vector<AstNode*>& NodeList,
    std::vector<Token>& TokenList,
    int& CurrentIndex,
    int& CurrentLine,
    int& CurrentCharacter
) {
    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    AstNode* LeftNode = ParseFactor(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    while (CurrentIndex < TokenList.size()) {
        Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

        if (CurrentToken.Variant != TokenVariant_Multiply && CurrentToken.Variant != TokenVariant_Divide) {
            break;
        }

        int OperatorLine = CurrentToken.LineIndex;
        int OperatorCharacter = CurrentToken.CharacterIndex;

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* RightNode = ParseFactor(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
        LeftNode = new OperatorNode(AstNodeType_OperatorNode, LeftNode, RightNode, CurrentToken.Variant, OperatorLine, OperatorCharacter);

    }

    return LeftNode;
}


AstNode* ParseExpression(
    std::string FilePath,
    std::vector<AstNode*>& NodeList,
    std::vector<Token>& TokenList,
    int& CurrentIndex,
    int& CurrentLine,
    int& CurrentCharacter
) {
    if (CurrentIndex >= TokenList.size()) {
        return nullptr;
    }

    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    AstNode* LeftNode = ParseTerm(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    while (CurrentIndex < TokenList.size()) {
        Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

        if (CurrentToken.Variant != TokenVariant_Add && CurrentToken.Variant != TokenVariant_Subtract) {
            break;
        }

        int OperatorLine = CurrentToken.LineIndex;
        int OperatorCharacter = CurrentToken.CharacterIndex;
        
        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* RightNode = ParseTerm(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
        AstNode* OperationNode = new OperatorNode(AstNodeType_OperatorNode, LeftNode, RightNode, CurrentToken.Variant, OperatorLine, OperatorCharacter);

        LeftNode = OperationNode;
    }

    return LeftNode;
}

