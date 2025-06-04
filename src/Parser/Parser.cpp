#include "Parser.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../utils.hpp"

#include <iostream>
#include <vector>

ProgramNode Program = ProgramNode(AstNodeType_ProgramNode, -1, -1);
std::vector<Token> Tokens = {};

int CurrentIndex = 0;
int CurrentLine = 1;

void Expect(TokenVariant TokenVariant, char StartingCharacter, char EndingCharacter) {
    for (int i=CurrentIndex; i<Tokens.size(); i++) {
        if (Tokens.at(i).Variant == TokenVariant) {
            return;
        }
    }

    log(-1, "[LINE: " + std::to_string(CurrentLine) + "] Expected '" + EndingCharacter + "' after '" + StartingCharacter + "'");
    exit(-1);
}

AstNode* GetTopNode(std::vector<AstNode*> *Body) {
    int BodySize = Body->size();

    if (BodySize > 0) {
        return Body->at(BodySize - 1);
    }

    return nullptr;
}

void ParseFactor(std::vector<AstNode*> *Body) {
    Token CurrentToken = Tokens.at(CurrentIndex);
    CurrentIndex += 1;

    AstNode* PastNode = GetTopNode(Body);
    AstNode* CreatedNode = nullptr;

    switch (CurrentToken.Type) {
        case TokenType_Number: 
            CreatedNode = new NumberNode(AstNodeType_NumberNode, CurrentToken.Variant, CurrentToken.Value, CurrentLine, CurrentToken.CharacterIndex);
            Body->push_back(CreatedNode);
            break;
        
        case TokenType_Operator:
            CreatedNode = new OperatorNode(AstNodeType_OperatorNode, PastNode, nullptr, CurrentToken.Variant, CurrentLine, CurrentToken.CharacterIndex);
            Body->push_back(CreatedNode);
            break;

        default:
            break;
    }

    if (PastNode != nullptr) {
        switch (PastNode->NodeType) {
            case AstNodeType_OperatorNode: {
                OperatorNode* CastedPastNode = static_cast<OperatorNode*>(PastNode);
                CastedPastNode->RightNode = CreatedNode;
                break;
            }

            default:
                break;
        }
    } 
}

void ParseTerm(std::vector<AstNode*> *Body) {
    ParseFactor(Body);
}

/*
    pair.first = ERRORS
    pair.second = WARNINGS
*/
std::pair<int, int> ValidateAstBody(std::vector<AstNode*> Body, std::string FilePath) {
    int Errors = 0;
    int Warnings = 0;

    for (AstNode *Node : Body) {
        switch (Node->NodeType) {
            case AstNodeType_OperatorNode: {
                OperatorNode *CastedNode = static_cast<OperatorNode*>(Node);

                if (CastedNode->RightNode == nullptr) {
                    std::string Spaces(CastedNode->CharacterIndex, ' ');

                    log(-1, "[LINE: " + std::to_string(CastedNode->CreationLine) + "] Expected an Expression after " + OperatorTypeToString(CastedNode->Operation) + " Operation"); 

                    std::cout << "\n> " + GetLineSnippet(FilePath, CastedNode->CreationLine) << std::endl;
                    std::cout << Spaces + " ^" << std::endl;
                    
                    Errors += 1;

                    break;
                }

                if (CastedNode->LeftNode->NodeType == AstNodeType_OperatorNode || CastedNode->LeftNode->NodeType == AstNodeType_OperatorNode) {
                    std::string Spaces(CastedNode->CharacterIndex, ' ');

                    std::string Order = "after";

                    if (CastedNode->LeftNode->NodeType == AstNodeType_OperatorNode) {
                        Order = "before";
                    }

                    if (CastedNode->LeftNode->NodeType == AstNodeType_OperatorNode && CastedNode->LeftNode->NodeType == AstNodeType_OperatorNode) {
                        Order = "on both sides of";
                    }

                    log(-1, "[LINE: " + std::to_string(CastedNode->CreationLine) + "] Expected a vaild Expression " + Order + " " + OperatorTypeToString(CastedNode->Operation) + " Operation"); 

                    std::cout << "\n> " + GetLineSnippet(FilePath, CastedNode->CreationLine) << std::endl;
                    std::cout << Spaces + " ^" << std::endl;
                    
                    Errors += 1;

                    break;
                }
                
                break;
            }
            default:
                break;
        }
    }

    return std::make_pair(Errors,Warnings);
}

void ValidateAst(AstNode* Node, std::string AstFileName) {
    int TotalErrors = 0;
    int TotalWarnings = 0;

    if (Node->NodeType == AstNodeType_ProgramNode) {
        ProgramNode* CastedNode = static_cast<ProgramNode*>(Node);

        for (ModuleNode Module : CastedNode->Modules) {
            ValidateAst(&Module, Module.FileName);
        }

       std::pair<int, int> Totals =  ValidateAstBody(CastedNode->Body, CastedNode->FilePath);

        TotalErrors = Totals.first;
        TotalWarnings = Totals.second;
    } else if (Node->NodeType == AstNodeType_ModuleNode) {
        ModuleNode* CastedNode = static_cast<ModuleNode*>(Node);

        std::pair<int, int> Totals = ValidateAstBody(CastedNode->Body, CastedNode->FilePath);

        TotalErrors = Totals.first;
        TotalWarnings = Totals.second;
    }

    std::cout << std::endl;

    if (TotalWarnings > 0) {
        log(0, "Total of " + std::to_string(TotalWarnings) + " Warnings Found in '" + AstFileName + "'");
    }

    if (TotalErrors > 0) {
        log(-1, "Total of " + std::to_string(TotalErrors) + " Errors Found in '" + AstFileName + "'");
        exit(-1);
    }
}


void ParseExpression(std::vector<AstNode*> *Body) {
    while (CurrentIndex < Tokens.size()) {
        if (Tokens.at(CurrentIndex).Variant == TokenVariant_NewLine) {
            CurrentLine += 1;
        }

        ParseFactor(Body);
    }

    ValidateAst(&Program, Program.FileName);
}

ProgramNode Parse(std::vector<Token> SourceTokens, std::string FileName, std::string FilePath) {
    Tokens = SourceTokens;

    Program.FileName = FileName;
    Program.FilePath = FilePath;

    ParseExpression(&Program.Body);

    return Program;
}