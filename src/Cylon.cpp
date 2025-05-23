#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#define Version "1.0.0"

#include "CommandLine\CommandLineInterface.hpp"
#include "CommandLine\Output.hpp"

#include "Lexer\Tokenizer.hpp"
#include "Lexer\Tokens.hpp"

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

    for (Token CurrentToken : SourceTokens) {
        std::cout << "TYPE: " + std::to_string(CurrentToken.Type) + " | VALUE: " + CurrentToken.Value << std::endl;
    }

    return 0;
}