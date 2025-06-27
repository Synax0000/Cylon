#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../Lexer/Tokens.hpp"
#include "DataTypes.hpp"


#ifndef ABSTRACTSYNTAXTREE_HPP
#define ABSTRACTSYNTAXTREE_HPP

typedef enum AstNodeType {
    AstNodeType_ProgramNode,
    AstNodeType_ModuleNode,
    AstNodeType_NumberNode,
    AstNodeType_OperatorNode,
    AstNodeType_GroupNode,
    AstNodeType_VariableDeclarationNode,
    AstNodeType_VariableAssignmentNode,
    AstNodeType_VariableNode,
} AstNodeType;

typedef struct AstNode {
    AstNodeType NodeType;

    int CreationLine;
    int CharacterIndex;

    AstNode() = default;
    AstNode(AstNodeType NodeTypeValue, int CreationLineValue, int CharacterIndexValue) {
        NodeType = NodeTypeValue;
        CreationLine = CreationLineValue;
        CharacterIndex = CharacterIndexValue;
    }
} AstNode;

typedef struct GroupNode : AstNode {
    std::vector<AstNode*> Members;

    GroupNode(
        AstNodeType NodeTypeValue, 
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Members = {};
    }
} GroupNode;

typedef struct NumberNode : AstNode {
    TokenVariant NumberType; 
    std::string Value;

    NumberNode(
        AstNodeType NodeTypeValue,TokenVariant NumberTypeValue, 
        std::string ValueValue, 
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        NumberType = NumberTypeValue;
        Value = ValueValue;
    }
} NumberNode; 

typedef struct VariableNode : AstNode {
    std::string Identifier;

    VariableNode(
        AstNodeType NodeTypeValue,
        std::string IdentifierValue, 
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Identifier = IdentifierValue;
    }
} VariableNode; 

typedef struct OperatorNode : AstNode {
    TokenVariant Operation;
    AstNode* LeftNode;
    AstNode* RightNode;

    OperatorNode(
        AstNodeType NodeTypeValue, 
        AstNode* Left,
        AstNode* Right, 
        TokenVariant OperationValue, 
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        LeftNode = Left;
        RightNode = Right;
        Operation = OperationValue;
    }
} OperatorNode;

typedef struct VariableAssignmentNode : AstNode {
    std::string Identifier;
    TokenVariant AssignmentOperation;
    AstNode* Assignment;

    VariableAssignmentNode(
        AstNodeType NodeTypeValue,
        std::string IdentifierValue, 
        AstNode* AssignmentValue,
        TokenVariant AssignmentOperationValue,
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Identifier = IdentifierValue;
        Assignment = AssignmentValue;
        AssignmentOperation = AssignmentOperationValue;
    }
} VariableAssignmentNode; 

typedef struct VariableDeclarationNode : AstNode {
    AstNode* VariableValue;
    std::string Identifier;
    std::vector<DataTypeVariant> Modifiers;
    DataTypeVariant DeclarationType;
    bool IsList;
    AstNode* ListSizeValue;

    VariableDeclarationNode(
        AstNodeType NodeTypeValue, 
        AstNode* VariableValueValue, 
        std::string IdentifierValue,
        std::vector<DataTypeVariant> ModifiersValue,
        DataTypeVariant DeclarationTypeValue,
        bool IsListValue,
        AstNode* ListSizeValueValue,
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        VariableValue = VariableValueValue;
        Identifier = IdentifierValue;
        Modifiers = ModifiersValue;
        DeclarationType = DeclarationTypeValue;
        IsList = IsListValue;
        ListSizeValue = ListSizeValueValue;
    }
} VariableDeclarationNode;

typedef struct ModuleNode : AstNode {
    std::string FileName;
    std::string FilePath;

    std::vector<AstNode*> Body;

    std::vector<Token> Tokens;

    ModuleNode(AstNodeType NodeTypeValue, 
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
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

    ProgramNode() : AstNode() {}
    ProgramNode(
        AstNodeType NodeTypeValue, 
        int CreationLineValue, 
        int CharacterIndexValue
    ) : AstNode(NodeTypeValue, CreationLineValue, CharacterIndexValue) {
        Modules = {};
        Body = {};
        Tokens = {};
        Nodes = {};
    }
} ProgramNode;


void VisualizeNode(AstNode *Node, int Indentation, int Limit);

#endif
