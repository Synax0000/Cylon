#include "AbstractSyntaxTree.hpp"
#include "../Utils.hpp"
#include <iostream>

void VisualizeNode(AstNode *Node, int Indentation, int Limit) {
    if (Indentation >= Limit) {
        std::cout << std::string(Indentation, '\t') << "..." << std::endl;
        return;
    }

    if (Node == nullptr) {
        std::cout << std::string(Indentation, '\t') << "EMPTY NODE:" << std::endl;
        return;
    }

    std::string Indentations = std::string(Indentation, '\t');

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

        case AstNodeType_GroupNode:
            std::cout << Indentations << "GROUP NODE:" << std::endl;
            break;

        case AstNodeType_VariableDeclarationNode:
            std::cout << Indentations << "VARIABLE DECLARATION NODE:" << std::endl;
            break;

        case AstNodeType_VariableAssignmentNode:
            std::cout << Indentations << "VARIABLE ASSIGNMENT NODE:" << std::endl;
            break;

        case AstNodeType_VariableNode:
            std::cout << Indentations << "VARIABLE NODE:" << std::endl;
            break;

        default:
            std::cout << Indentations << "UNKNOWN NODE:" << std::endl;
            break;
    }

    std::cout << Indentations << " Creation Line: " << Node->CreationLine << std::endl;
    std::cout << Indentations << " Character Index: " << (Node->CharacterIndex + 1) << std::endl;

    switch (Node->NodeType) {
        case AstNodeType_ProgramNode: {
            ProgramNode *CastedNode = static_cast<ProgramNode*>(Node);

            std::cout << Indentations << " Total Modules: " << CastedNode->Modules.size() << std::endl;
            
            if (!CastedNode->Modules.empty()) {
                std::cout << Indentations << " Module Nodes:" << std::endl;
                for (ModuleNode &Module : CastedNode->Modules) {
                    
                    VisualizeNode(&Module, Indentation + 1, Limit);
                    std::cout << std::endl;
                }
            }

            std::cout << std::endl << Indentations << " CHILDREN:" << std::endl;

            for (AstNode *Child : CastedNode->Body) {
                VisualizeNode(Child, Indentation + 1, Limit);
                std::cout << std::endl;
            }

            break;
        }

        case AstNodeType_ModuleNode: {
            ModuleNode *CastedNode = static_cast<ModuleNode*>(Node);

            if (!CastedNode->Body.empty()) {
                std::cout << std::endl << Indentations << " CHILDREN:" << std::endl;
                for (AstNode *Child : CastedNode->Body) {
                    VisualizeNode(Child, Indentation + 1, Limit);
                    std::cout << std::endl;
                }
            }
            break;
        }

        case AstNodeType_NumberNode: {
            NumberNode *CastedNode = static_cast<NumberNode*>(Node);

            std::cout << Indentations << " Type: " << NumberTypeToString(CastedNode->NumberType) << std::endl;
            std::cout << Indentations << " Value: " << CastedNode->Value << std::endl;

            break;
        }

        case AstNodeType_OperatorNode: {
            OperatorNode *CastedNode = static_cast<OperatorNode*>(Node);

            std::cout << Indentations << " Type: " << OperatorTypeToString(CastedNode->Operation) << std::endl;

            std::cout << Indentations << " Left Node:" << std::endl;
            VisualizeNode(CastedNode->LeftNode, Indentation + 1, Limit);

            std::cout << Indentations << " Right Node:" << std::endl;
            VisualizeNode(CastedNode->RightNode, Indentation + 1, Limit);

            break;
        }

        case AstNodeType_GroupNode: {
            GroupNode *CastedNode = static_cast<GroupNode*>(Node);

            if (!CastedNode->Members.empty()) {
                std::cout << std::endl << Indentations << " MEMBERS:" << std::endl;

                for (AstNode *Member : CastedNode->Members) {
                    VisualizeNode(Member, Indentation + 1, Limit);
                    std::cout << std::endl;
                }
            }

            break;
        }

        case AstNodeType_VariableDeclarationNode: {
            VariableDeclarationNode *CastedNode = static_cast<VariableDeclarationNode*>(Node);
            
            std::cout << std::endl << Indentations << " Modifiers:" << std::endl;

            for (DataTypeVariant Variant : CastedNode->Modifiers) {
                std::cout << Indentations << "   " + Variant.Value << std::endl;
            }
            
            std::cout << std::endl << Indentations << " Declaration Type: " + CastedNode->DeclarationType.Value<< std::endl;
            std::cout << Indentations << " Identifier: " + CastedNode->Identifier << std::endl << std::endl;

            std::cout << Indentations << " IsList: " + BoolToYesNo(CastedNode->IsList) << std::endl;
            std::cout << std::endl << Indentations << " List Size Value: " << std::endl;
            
            VisualizeNode(CastedNode->ListSizeValue, Indentation + 1, Limit);
            
            std::cout << std::endl << Indentations << " Value:" << std::endl;

            VisualizeNode(CastedNode->VariableValue, Indentation + 1, Limit);

            break;
        }

        case AstNodeType_VariableAssignmentNode: {
            VariableAssignmentNode *CastedNode = static_cast<VariableAssignmentNode*>(Node);

            std::cout << Indentations << " Identifier: " + CastedNode->Identifier << std::endl;
            std::cout << Indentations << " Operation: " + TokenOperationToString(CastedNode->AssignmentOperation) << std::endl << std::endl;
            std::cout << Indentations << " Value:" << std::endl;

            VisualizeNode(CastedNode->Assignment, Indentation + 1, Limit);

            break;
        }

        case AstNodeType_VariableNode: {
            VariableNode *CastedNode = static_cast<VariableNode*>(Node);

            std::cout << Indentations << " Identifier: " + CastedNode->Identifier << std::endl << std::endl;

            break;
        }

        default:
            break;
    }
}
