#include "AbstractSyntaxTree.hpp"

#include "../utils.hpp"

void VisualizeNode(AstNode *Node, int Indentation) {
    std::string Indentations(Indentation, '\t');

    if (Node == nullptr) {
        std::cout << Indentations << "EMPTY NODE:" << std::endl;
        return;
    }

    switch (Node->NodeType) {
        case AstNodeType_ProgramNode:
            std::cout << Indentations << "PROGRAM NODE:" << std::endl;
            break;

        case AstNodeType_ModuleNode:
            std::cout << Indentations << "MODULE NODE:" << std::endl;
            break;

        case AstNodeType_NumberNode:
            std::cout << Indentations << "NUMBER NODE:" << std::endl;
            break;

        case AstNodeType_OperatorNode:
            std::cout << Indentations << "OPERATOR NODE:" << std::endl;
            break;

        default:
            std::cout << Indentations << "UNKNOWN NODE:" << std::endl;
            break;
    }

    std::cout << Indentations << " Creation Line: " + std::to_string(Node->CreationLine) << std::endl;
    std::cout << Indentations << " Character Index: " + std::to_string(Node->CharacterIndex) << std::endl;

    switch (Node->NodeType) {
        case AstNodeType_ProgramNode: {
            ProgramNode *CastedNode = static_cast<ProgramNode*>(Node);

            std::cout << Indentations << " Total Modules: " + CastedNode->Modules.size() << std::endl;
            std::cout << Indentations << " Module Nodes:" << std::endl;

            for (ModuleNode Module : CastedNode->Modules) {
                VisualizeNode(&Module, Indentation + 1);
            }

            break;
        }

        case AstNodeType_ModuleNode: {
            ModuleNode *CastedNode = static_cast<ModuleNode*>(Node);
            break;
        }

        case AstNodeType_NumberNode: {
            NumberNode *CastedNode = static_cast<NumberNode*>(Node);

            std::cout << Indentations << " Type: " + NumberTypeToString(CastedNode->NumberType) << std::endl;
            std::cout << Indentations << " Value: " + CastedNode->Value << std::endl;

            break;
        }

        case AstNodeType_OperatorNode: {
            OperatorNode *CastedNode = static_cast<OperatorNode*>(Node);

            std::cout << Indentations << " Type: " + OperatorTypeToString(CastedNode->Operation) << std::endl;

            std::cout << Indentations << " Left Node:" << std::endl;
            VisualizeNode(CastedNode->LeftNode, Indentation+1);

            std::cout << Indentations << " Right Node:" << std::endl;
            VisualizeNode(CastedNode->RightNode, Indentation+1);

            break;
        }

        default:
            break;
    }

    std::cout << std::endl << Indentations << " CHILDREN:" << std::endl;

    Indentations += "\t";
    Indentation += 1;

    if (Node->NodeType == AstNodeType_ProgramNode) {
        ProgramNode *CastedNode = static_cast<ProgramNode*>(Node);

        for (AstNode *CurrentNode : CastedNode->Body) {
            VisualizeNode(CurrentNode, Indentation);
            std::cout << std::endl;
        }
    } else if (Node->NodeType == AstNodeType_ModuleNode) {
        ModuleNode *CastedNode = static_cast<ModuleNode*>(Node);

        for (AstNode *CurrentNode : CastedNode->Body) {
            VisualizeNode(CurrentNode, Indentation);
            std::cout << std::endl;
        }
    } else {
        for (AstNode *CurrentNode : Node->Children) {
            VisualizeNode(CurrentNode, Indentation);
            std::cout << std::endl;
        } 
    }
}