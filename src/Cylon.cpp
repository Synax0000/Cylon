#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "config.hpp"

#include "CommandLine\CommandLineInterface.hpp"
#include "CommandLine\Output.hpp"

#include "Lexer\Tokenizer.hpp"
#include "Lexer\Tokens.hpp"

#include "Parser\AbstractSyntaxTree.hpp"
#include "Parser\Parser.hpp"

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
    
    std::string InputFile = GetFlag(CFLAGS, CFLAG_INPUTFILE).Value;

    if (InputFile == "") {
        log(-1, "No input file provided");
        exit(-1);
    }

    std::ifstream File(InputFile);

    if (!File.is_open()) {
        log(-1, "Input file doesnt exist");
        exit(-1);
    }

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

    std::vector<Token> SourceTokens = Tokenize(Source);

    size_t LastSlash = InputFile.find_last_of("/\\");
    std::string FileName = InputFile.substr(LastSlash + 1);

    ProgramNode AstTree = Parse(SourceTokens, FileName, InputFile);

    log(2, "Displaying generated Ast Tree");

    if (DEBUG == true) {
        std::cout << std::endl;
        VisualizeNode(&AstTree, 0);
    }

    return 0;
}