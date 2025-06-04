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
} AstNodeType;

typedef struct AstNode {
    AstNodeType NodeType;
    std::vector<AstNode*> Children;
    int CreationLine;
    int CharacterIndex;

    AstNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) {
        Children = {};

        NodeType = NodeTypeValue;
        CreationLine = CreationLineValue;
        CharacterIndex = CharacterIndexValue;
    }
} AstNode;

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

    ModuleNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Body = {};
    }
} ModuleNode;

typedef struct ProgramNode : AstNode {
    std::string FileName;
    std::string FilePath;

    std::vector<ModuleNode> Modules;
    std::vector<AstNode*> Body;

    ProgramNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Modules = {};
        Body = {};
    }
} ProgramNode;


void VisualizeNode(AstNode *Node, int Indentation);

#endif
