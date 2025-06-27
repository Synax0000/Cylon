#include "ParseVariable.hpp"

#include "AbstractSyntaxTree.hpp"

#include "../Lexer/Tokens.hpp"
#include "../CommandLine/Output.hpp"
#include "../Utils.hpp"

#include "DataTypes.hpp"
#include "ParserUtils.hpp"

#include "ParseExpression.hpp"



std::vector<std::string> ParseVariableIdentifiers(std::string FilePath, std::vector<Token>& TokenList, int &CurrentIndex, int &CurrentLine, int &CurrentCharacter) {
    std::vector<std::string> VariableIdentifiers;

    while (CurrentIndex < TokenList.size()) {
        Token PresentToken = GetCurrentToken(TokenList, CurrentIndex);
        Token FutureToken = PeekNext(TokenList, CurrentIndex);

        if (PresentToken.Variant == TokenVariant_NewLine) {
            AdvanceToken(CurrentIndex, TokenList);
            continue;
        }

        if (PresentToken.Type != TokenType_Identifier) {
            UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
            logsnippet(-1, FilePath, "Expected an identifier after comma", CurrentLine, CurrentCharacter);
            exit(-1);
        } 

        VariableIdentifiers.push_back(PresentToken.Value);

        if (FutureToken.Type == TokenType_Operator || FutureToken.Variant == TokenVariant_SemiColon || FutureToken.Variant == TokenVariant_SquareBracket) {
            if (FutureToken.Variant == TokenVariant_SemiColon) {
                AdvanceToken(CurrentIndex, TokenList);
            } else {
                if (FutureToken.Variant == TokenVariant_SquareBracket) {
                    AdvanceToken(CurrentIndex, TokenList);
                } else {
                    Advance(CurrentIndex, TokenList, 2);

                    if (GetCurrentToken(TokenList, CurrentIndex).Type == TokenType_Operator) {
                        AdvanceToken(CurrentIndex, TokenList);
                    }
                }
            }

            break;
        } else if (FutureToken.Variant == TokenVariant_Comma) {
            Advance(CurrentIndex, TokenList, 2);
        } else {
            AdvanceToken(CurrentIndex, TokenList);

            UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
            logsnippet(-1, FilePath, "Expected a valid symbol after variable Identifier", CurrentLine, CurrentCharacter);
            exit(-1);
        }
        
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    }

    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    
    return VariableIdentifiers;
}


std::vector<DataTypeVariant> ParseVariableModifiers(std::string FilePath, std::vector<Token>& TokenList, int &CurrentIndex, int &CurrentLine, int &CurrentCharacter) {
    std::vector<DataTypeVariant> Modifiers;

    while (CurrentIndex < TokenList.size()) {
        Token PresentToken = GetCurrentToken(TokenList, CurrentIndex);

        if (PresentToken.Variant == TokenVariant_NewLine) {
            AdvanceToken(CurrentIndex, TokenList);
            continue;
        }


        if (PresentToken.Type != TokenType_Identifier) {
            AdvanceToken(CurrentIndex, TokenList);
            logsnippet(-1, FilePath, "Expected a vaild variable idenifier after variable modifier", CurrentLine, CurrentCharacter);
            exit(-1);
        }

        DataTypeVariant Variant = StringToDataTypeVariant(PresentToken.Value);

        if (Variant.Value == "Invalid") {
            break;
        }

        Modifiers.push_back(Variant);

        AdvanceToken(CurrentIndex, TokenList);
        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    }

    if (Modifiers.empty()) {
        logsnippet(-1, FilePath, "Expected at least one modifier or type identifier", CurrentLine, CurrentCharacter);
        exit(-1);
    }

    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    return Modifiers;
}



std::vector<AstNode*> ParseVariableValues(std::string FilePath, std::vector<AstNode*> NodeList, std::vector<Token>& TokenList, int &CurrentIndex, int &CurrentLine, int &CurrentCharacter) {
    std::vector<AstNode*> VariableValues;

    if (GetCurrentToken(TokenList, CurrentIndex).Variant == TokenVariant_SemiColon) {
        VariableValues.push_back(nullptr);
    } else {
        std::vector<Token> ValueTokens = ConsumeFullTokenRange(TokenList, CurrentIndex, ";", true);
        
        if (ContainsToken(ValueTokens, TokenVariant_Comma) == true) {
            std::vector<std::vector<Token>> TokenLists;

            int StartingIndex = 0;
            
            while (true) {
                int Index = FindIndexOfToken(ValueTokens, StartingIndex, TokenVariant_Comma);

                if (Index == -1) {
                    TokenLists.push_back(ConsumeFullTokenRange(ValueTokens, StartingIndex, ";", false));

                    break;
                } else {
                    TokenLists.push_back(ConsumeFullTokenRange(ValueTokens, StartingIndex, ",", false));
                    StartingIndex = Index + 1;
                }
            }

            for (std::vector<Token> List : TokenLists) {
                int ExpressionIndex = 0;

                AstNode* VariableValue = ParseExpression(FilePath, NodeList, List, ExpressionIndex, CurrentLine, CurrentCharacter);
                NodeList.push_back(VariableValue);
                VariableValues.push_back(VariableValue);
            }
        } else {
            int ExpressionIndex = 0;

            AstNode* VariableValue = ParseExpression(FilePath, NodeList, ValueTokens, ExpressionIndex, CurrentLine, CurrentCharacter);
            NodeList.push_back(VariableValue);
            VariableValues.push_back(VariableValue);
        }
    }

    return VariableValues;
}


AstNode* ParseDeclaration(
    std::string FilePath, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int &CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
) {
    std::vector<DataTypeVariant> Modifiers;

    int CharacterIndex = CurrentCharacter;
    int LineIndex = CurrentLine;
    
    Modifiers = ParseVariableModifiers(FilePath, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    std::vector<std::string> VariableIdentifiers = ParseVariableIdentifiers(FilePath, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);   

    Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

    AstNode* ListValue;
    bool IsList = false;

    if (CurrentToken.Variant == TokenVariant_SquareBracket) {
        IsList = true;

        if (CurrentToken.Value == "[") {
            if (MatchToken(TokenList, CurrentIndex, "]") == false) {
                logsnippet(-1, FilePath, "Expected matching ']' for '['", CurrentLine, CurrentCharacter);
                exit(-1);
            }

            std::vector<Token> ListTokenList = ConsumeTokenRange(TokenList, CurrentIndex, "]");

            int ListIndex = 0;
            ListValue = ParseExpression(FilePath,NodeList, ListTokenList, ListIndex, CurrentLine, CurrentCharacter);
        } else {
            logsnippet(-1, FilePath, "Expected '[' for list identifier", CurrentLine, CurrentCharacter);
            exit(-1);
        }
    } 

    AdvanceToken(CurrentIndex, TokenList);
    std::vector<AstNode*> VariableValues = ParseVariableValues(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    GroupNode* Declarations = new GroupNode(AstNodeType_GroupNode, LineIndex, CharacterIndex);
    NodeList.push_back(Declarations);

    DataTypeVariant Type;

    for (auto Iterator = Modifiers.begin(); Iterator != Modifiers.end(); ) {
        if (ValidType(Iterator->Value, true) == true) {
            Type = *Iterator;
            Iterator = Modifiers.erase(Iterator);
            break;
        } else {
            ++Iterator;
        }
    }

    for (int i=0; i<VariableIdentifiers.size(); i++) {
        AstNode* VariableValue;
        std::string VariableIdentifier = VariableIdentifiers[i];

        if (VariableValues.size() == 1) {
            VariableValue = VariableValues[0];
        } else {
            VariableValue = VariableValues[i];
        }

        VariableDeclarationNode* DeclarationNode = new VariableDeclarationNode(AstNodeType_VariableDeclarationNode, VariableValue, VariableIdentifier, Modifiers, Type, IsList, ListValue, LineIndex, CharacterIndex);
        
        Declarations->Members.push_back(DeclarationNode);
    }

    if (VariableIdentifiers.size() == 1) {
        return Declarations->Members.at(0);
    } else {
        return Declarations;
    }
}

AstNode* ParseAssignment(
    std::string FilePath, 
    std::vector<AstNode*>& NodeList, 
    std::vector<Token>& TokenList, 
    int& CurrentIndex, 
    int &CurrentLine, 
    int &CurrentCharacter
) {
    std::vector<std::string> VariableIdentifiers;
    std::string VariableOperationString = "";

    int CharacterIndex = CurrentCharacter;
    int LineIndex = CurrentLine;

    {
        Token CurrentToken = GetCurrentToken(TokenList, CurrentIndex);

        if (PeekNext(TokenList, CurrentIndex).Variant == TokenVariant_Comma) {
            UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

            VariableIdentifiers = ParseVariableIdentifiers(FilePath, TokenList, CurrentIndex, CurrentLine, CurrentCharacter); 
        } else {
            VariableIdentifiers.push_back(GetCurrentToken(TokenList, CurrentIndex).Value);

            if (PeekNext(TokenList, CurrentIndex).Type == TokenType_Operator) {
                if (Peek(TokenList, CurrentIndex, 2).Type == TokenType_Operator) {
                    AdvanceToken(CurrentIndex, TokenList);
                }

                Advance(CurrentIndex, TokenList, 2);
            }
        }

        if (PeekBehind(TokenList, CurrentIndex).Type == TokenType_Operator) {
            if (Backtrack(TokenList, CurrentIndex, 2).Type == TokenType_Operator) {
                VariableOperationString += Backtrack(TokenList, CurrentIndex, 2).Value;
            }

            VariableOperationString += PeekBehind(TokenList, CurrentIndex).Value;
        }

        UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    }

    TokenVariant Operation = StringToTokenOperation(VariableOperationString);

    std::vector<AstNode*> VariableValues = ParseVariableValues(FilePath, NodeList, TokenList, CurrentIndex, CurrentLine, CurrentCharacter);
    
    UpdateCurrentLocation(TokenList, CurrentIndex, CurrentLine, CurrentCharacter);

    GroupNode* Assignments = new GroupNode(AstNodeType_GroupNode, LineIndex, CharacterIndex);
    NodeList.push_back(Assignments);

    for (int i=0; i<VariableIdentifiers.size(); i++) {
        AstNode* VariableValue;
        std::string VariableIdentifier = VariableIdentifiers[i];

        if (VariableValues.size() == 1) {
            VariableValue = VariableValues[0];
        } else {
            if (i+1 > VariableValues.size()) {
                VariableValue = nullptr;
            } else {
                VariableValue = VariableValues[i];
            }
            
        }

        VariableAssignmentNode* AssignmentNode = new VariableAssignmentNode(AstNodeType_VariableAssignmentNode, VariableIdentifier, VariableValue, Operation, LineIndex, CharacterIndex);
        
        NodeList.push_back(AssignmentNode);
        Assignments->Members.push_back(AssignmentNode);
    }

    if (VariableIdentifiers.size() == 1) {
        return Assignments->Members.at(0);
    } else {
        return Assignments;
    }
}