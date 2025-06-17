#include "ParseExpression.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#include "ParserUtils.hpp"

AstNode* ParseFactor(
    std::string FilePath,
    std::vector<AstNode*>& Body,
    std::vector<AstNode*>& NodeList,
    std::vector<Token>& TokenList,
    int& CurrentIndex,
    int& CurrentLine,
    int& CurrentCharacter
) {
    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

    if (CurrentToken.Type == TokenType_Number) {
        AstNode* CreatedNode = new NumberNode(AstNodeType_NumberNode, CurrentToken.Variant, CurrentToken.Value, CurrentLine, CurrentCharacter);
        NodeList.push_back(CreatedNode);
        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        if (CurrentIndex < TokenList.size() && GetCurrentToken(TokenList, CurrentIndex).Variant == TokenVariant_Power) {
            int OperatorLine = GetCurrentToken(TokenList, CurrentIndex).LineIndex;
            int OperatorCharacter = GetCurrentToken(TokenList, CurrentIndex).CharacterIndex;

            AdvanceToken(CurrentIndex, TokenList);
            UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

            AstNode* RightNode = ParseFactor(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
            AstNode* OperationNode = new OperatorNode(AstNodeType_OperatorNode, CreatedNode, RightNode, TokenVariant_Power, OperatorLine, OperatorCharacter);
            Body.push_back(OperationNode);
            return OperationNode;
        }
        return CreatedNode;
    }

    if (CurrentToken.Variant == TokenVariant_Parentheses && CurrentToken.Value == "(") {
        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* InnerNode = ParseExpression(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        if (CurrentIndex >= (int)TokenList.size() || GetCurrentToken(TokenList, CurrentIndex).Value != ")") {
            logsnippet(-1, FilePath, "Expected closing parenthesis", CurrentLine, CurrentCharacter);
            exit(-1);
        }

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* GroupedNode = new GroupNode(AstNodeType_GroupNode, CurrentToken.LineIndex, CurrentToken.CharacterIndex);

        GroupNode* CastedNode = static_cast<GroupNode*>(GroupedNode);
        CastedNode->Members.push_back(InnerNode);

        NodeList.push_back(GroupedNode);

        if (CurrentIndex < TokenList.size() && GetCurrentToken(TokenList, CurrentIndex).Variant == TokenVariant_Power) {
            int OperatorLine = GetCurrentToken(TokenList, CurrentIndex).LineIndex;
            int OperatorCharacter = GetCurrentToken(TokenList, CurrentIndex).CharacterIndex;

            AdvanceToken(CurrentIndex, TokenList);
            UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

            AstNode* RightNode = ParseFactor(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
            AstNode* OperationNode = new OperatorNode(AstNodeType_OperatorNode, GroupedNode, RightNode, TokenVariant_Power, OperatorLine, OperatorCharacter);
            Body.push_back(OperationNode);
            return OperationNode;
        }

        return GroupedNode;
    }

    logsnippet(-1, FilePath, "Expected number or opening parentheses", CurrentLine, CurrentCharacter);
    return nullptr;
}

AstNode* ParseTerm(
    std::string FilePath,
    std::vector<AstNode*>& Body,
    std::vector<AstNode*>& NodeList,
    std::vector<Token>& TokenList,
    int& CurrentIndex,
    int& CurrentLine,
    int& CurrentCharacter
) {
    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    AstNode* LeftNode = ParseFactor(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    while (CurrentIndex < TokenList.size()) {
        Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

        if (CurrentToken.Variant != TokenVariant_Multiply && CurrentToken.Variant != TokenVariant_Divide) {
            break;
        }

        int OperatorLine = CurrentToken.LineIndex;
        int OperatorCharacter = CurrentToken.CharacterIndex;
        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* RightNode = ParseFactor(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
        AstNode* OperationNode = new OperatorNode(AstNodeType_OperatorNode, LeftNode, RightNode, CurrentToken.Variant, OperatorLine, OperatorCharacter);

        Body.push_back(OperationNode);
        LeftNode = OperationNode;
    }

    return LeftNode;
}

AstNode* ParseExpression(
    std::string FilePath,
    std::vector<AstNode*>& Body,
    std::vector<AstNode*>& NodeList,
    std::vector<Token>& TokenList,
    int& CurrentIndex,
    int& CurrentLine,
    int& CurrentCharacter
) {
    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    AstNode* LeftNode = ParseTerm(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    while (CurrentIndex < TokenList.size()) {
        Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

        if (CurrentToken.Variant != TokenVariant_Add && CurrentToken.Variant != TokenVariant_Subtract) {
            break;
        }

        int OperatorLine = CurrentToken.LineIndex;
        int OperatorCharacter = CurrentToken.CharacterIndex;
        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

        AstNode* RightNode = ParseTerm(FilePath, Body, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
        AstNode* OperationNode = new OperatorNode(AstNodeType_OperatorNode, LeftNode, RightNode, CurrentToken.Variant, OperatorLine, OperatorCharacter);

        NodeList.push_back(OperationNode);
        LeftNode = OperationNode;
    }

    return LeftNode;
}
