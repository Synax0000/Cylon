#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../Lexer/Tokens.hpp"

#ifndef ABSTRACTSYNTAXTREE_HPP
#define ABSTRACTSYNTAXTREE_HPP

typedef enum AstNodeType {
    AstNodeType_ProgramNode,
    AstNodeType_ModuleNode,
    AstNodeType_NumberNode,
    AstNodeType_OperatorNode,
    AstNodeType_GroupNode,
} AstNodeType;

typedef struct AstNode {
    AstNodeType NodeType;

    int CreationLine;
    int CharacterIndex;

    AstNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) {
        NodeType = NodeTypeValue;
        CreationLine = CreationLineValue;
        CharacterIndex = CharacterIndexValue;
    }
} AstNode;

typedef struct GroupNode : AstNode {
    std::vector<AstNode*> Members;

    GroupNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Members = {};
    }
} GroupNode;

typedef struct NumberNode : AstNode {
    TokenVariant NumberType; 
    std::string Value;

    NumberNode(AstNodeType NodeTypeValue,TokenVariant NumberTypeValue, std::string ValueValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        NumberType = NumberTypeValue;
        Value = ValueValue;
    }
} NumberNode; 

typedef struct OperatorNode : AstNode {
    TokenVariant Operation;
    AstNode* LeftNode;
    AstNode* RightNode;

    OperatorNode(AstNodeType NodeTypeValue,AstNode* Left,AstNode* Right, TokenVariant OperationValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        LeftNode = Left;
        RightNode = Right;
        Operation = OperationValue;
    }
} OperatorNode;

typedef struct ModuleNode : AstNode {
    std::string FileName;
    std::string FilePath;

    std::vector<AstNode*> Body;

    std::vector<Token> Tokens;

    ModuleNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Body = {};
        Tokens = {};
    }
} ModuleNode;

typedef struct ProgramNode : AstNode {
    std::string FileName;
    std::string FilePath;

    std::vector<ModuleNode> Modules;

    std::vector<AstNode*> Nodes;
    std::vector<AstNode*> Body;

    std::vector<Token> Tokens;

    ProgramNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Modules = {};
        Body = {};
        Tokens = {};
        Nodes = {};
    }
} ProgramNode;


void VisualizeNode(AstNode *Node, int Indentation, int Limit);

#endif
