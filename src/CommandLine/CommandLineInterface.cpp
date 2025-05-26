#include "CommandLineInterface.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "Output.hpp"

std::vector<Command> Commands = {
    Command(
        CFLAG_INPUTFILE,
        true,
        {"-i","--input"},
        "Tells the Compiler which file to compile as the Main Source"
    ),

    Command(
        CFLAG_OUTPUTFILE,
        true,
        {"-o","--output"},
        "Tells the Compiler what the Compiled Executable should be named"
    ),

    Command(
        CFLAG_HELP,
        false,
        {"-h","--help"},
        "Displays all available Commands"
    ),

    Command(
        CFLAG_VERSION,
        false,
        {"-v","--version"},
        "Displays current Cylon Version"
    ),

    Command(
        CFLAG_DEBUG,
        false,
        {"-d","--debug"},
        "Enables the debug output for Cylon"
    ),
};

Command IsCommand(std::string Text) {
    for (Command CurrentCommand : Commands) {
        for (std::string Usage : CurrentCommand.Usages) {
            if (Usage == Text) {
                return CurrentCommand;
            }
        }
    }

    return Command(CFLAG_INVALID, NULL, {}, "");
}

std::vector<CFLAG> GetFlags(int ArgumentCount, char** ArgumentValues) {
    std::vector<CFLAG> Flags = {};

    for (int Index = 1; Index < ArgumentCount; Index++) {
        std::string Text = std::string(ArgumentValues[Index]);

        Command CurrentCommand = IsCommand(Text);

        if (CurrentCommand.Type != CFLAG_INVALID) {
            std::string Value;

            if (CurrentCommand.RequiresValue == true) {
                if (Index + 1 >= ArgumentCount) {
                    log(-1, "No value passed for command \"" + Text + "\"");
                    exit(-1);
                }

                Value = ArgumentValues[Index + 1];

                if (IsCommand(Value).Type != CFLAG_INVALID) {
                    log(-1, "Command was passed to Command \"" + Text + "\" as a value, Commands are not allowed as values");
                    exit(-1);
                }

                Index++;
            }

            Flags.push_back(CFLAG(CurrentCommand.Type, Value));

        } else {
            log(-1, "Unknown command \"" + Text + "\", use '" + std::string(ArgumentValues[0]) + " --help' for more information on available commands");
            exit(-1);
        }
    }

    if (Flags.size() == 0) {
        log(-1, "No commands passed, use '" + std::string(ArgumentValues[0]) + " --help' for more information on available commands");
        exit(-1);
    }

    return Flags;
}

CFLAG GetFlag(std::vector<CFLAG> Flags, CFLAGTYPE Type) {
    for (CFLAG Flag : Flags) {
        if (Flag.Type == Type) {
            return Flag;
        }
    }

    return CFLAG(CFLAG_INVALID, "");
}