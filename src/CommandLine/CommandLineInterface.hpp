#include <vector>
#include <string>

#ifndef COMMANDLINEINTERFACE_HPP
#define COMMANDLINEINTERFACE_HPP

typedef enum CFLAGTYPE {
    CFLAG_INVALID = -1,

    CFLAG_INPUTFILE,
    CFLAG_OUTPUTFILE,

    CFLAG_HELP,
    CFLAG_VERSION
} CFLAGTYPE;

typedef struct CFLAG {
    CFLAGTYPE Type;
    std::string Value;

    CFLAG(CFLAGTYPE TypeValue, std::string FlagValue) {
        Type = TypeValue;
        Value = FlagValue;
    }
} CFLAG;

typedef struct Command {
    CFLAGTYPE Type;
    bool RequiresValue;
    std::vector<std::string> Usages;
    std::string Description;

    Command(CFLAGTYPE TypeValue, bool RequiresValueArgument, std::vector<std::string> UsagesValues, std::string DescriptionValue) {
        Type = TypeValue;
        RequiresValue = RequiresValueArgument;
        Usages = UsagesValues;
        Description = DescriptionValue;
    }
} Command;

extern std::vector<Command> Commands;

std::vector<CFLAG> GetFlags(int argc, char* argv[]);
CFLAG GetFlag(std::vector<CFLAG> Flags, CFLAGTYPE Type);

#endif