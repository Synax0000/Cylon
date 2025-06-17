#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "Config.hpp"

#include "CommandLine\CommandLineInterface.hpp"
#include "CommandLine\Output.hpp"

#include "Lexer\Tokenizer.hpp"
#include "Lexer\Tokens.hpp"

#include "Parser\AbstractSyntaxTree.hpp"
#include "Parser\Parser.hpp"

#include "Utils.hpp"

int main(int argc, char* argv[]) {
    std::vector<CFLAG> CFLAGS = GetFlags(argc,argv);

    for (CFLAG Flag : CFLAGS) {
        switch (Flag.Type)
        {
            case CFLAG_HELP:
                std::cout << "Cylon Commands:" << std::endl << std::endl; 

                for (Command CurrentCommand : Commands) {
                    std::string ConstructedString = "";
                    ConstructedString += "\tCommand: " + CurrentCommand.Usages[1] + "\n";
                    ConstructedString += "\t   Usages: " + CurrentCommand.Usages[0] + ", " + CurrentCommand.Usages[1] + "\n";
                    
                    std::string RequiresValue;

                    if (CurrentCommand.RequiresValue == true) {
                        RequiresValue = "Yes";
                    } else {
                        RequiresValue = "No";
                    }

                    ConstructedString += "\t   Requires Value: " + RequiresValue + "\n";
                    ConstructedString += "\t   Description: " + CurrentCommand.Description + "\n";

                    std::cout << ConstructedString << std::endl;
                }

                break;

            case CFLAG_VERSION:
                std::cout << Version << std::endl;
                break;

            case CFLAG_DEBUG:
                DEBUG = true;
                break;
            
            default:
                break;
        }
    }
    
    log(2, "Getting input file from cflag");
    std::string InputFile = GetFlag(CFLAGS, CFLAG_INPUTFILE).Value;

    if (InputFile == "") {
        log(-1, "No input file provided");
        exit(-1);
    }

    log(2, "Opening input file");
    std::ifstream File(InputFile);

    if (!File.is_open()) {
        log(-1, "Input file doesnt exist");
        exit(-1);
    }

    log(2, "Reading file contents");
    std::ostringstream StringStream;
    StringStream << File.rdbuf();

    if (File.fail() || File.bad())
    {
        log(-1, "Input file is not readable");
        exit(-1);
    }

    std::string Source = StringStream.str();

    File.close();

    if (InputFile.size() < 3 || InputFile.substr(InputFile.size() - 3) != ".cy") {
        log(-1, "Input file is required to be a '.cy' type");
        exit(-1);
    }


    log(2, "Tokenizing Source '" + InputFile + "'");

    std::vector<Token> SourceTokens = Tokenize(Source, InputFile);
    
    log(2, "Displaying generated tokens");

    if (DEBUG == true) { 
        for (Token CurrentToken : SourceTokens) {
            std::cout << "\n" + TokenTypeToString(CurrentToken.Variant) + ":" << std::endl;
            std::cout << " Line: " + std::to_string(CurrentToken.LineIndex) << std::endl;
            std::cout << " Character: " + std::to_string(CurrentToken.CharacterIndex + 1) << std::endl;
            std::cout << " Value: " + CurrentToken.Value << std::endl;
        }

        std::cout << std::endl;
    }

    size_t LastSlash = InputFile.find_last_of("/\\");
    std::string FileName = InputFile.substr(LastSlash + 1);

    ProgramNode AstTree = Parse(SourceTokens, FileName, InputFile);

    log(2, "Displaying generated Ast Tree");

    if (DEBUG == true) {
        std::cout << std::endl;
        VisualizeNode(&AstTree, 0, 12);
    }

    return 0;
}