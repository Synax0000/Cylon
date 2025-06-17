#include "Output.hpp"

#include <iostream>
#include <string>

#include <Windows.h>

#include "../Config.hpp"
#include "../Utils.hpp"

void log(int LogLevel, std::string Text) {
    HANDLE StandardHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string FullText;

    switch (LogLevel) {
        case -1:
            SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            FullText += "[ERROR] ";
            break;
        case 0:
            SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            FullText += "[WARNING] ";
            break;
        case 1:
            SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            FullText += "[INFO] ";
            break;
        case 2:
            if (DEBUG == false) {
                return;
            }

            SetConsoleTextAttribute(StandardHandle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
            FullText += "[DEBUG] ";
            break;
        default:
            SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "[Invalid Log Level Passed]" << std::endl;
            return;
    }

    FullText += Text + ".";

    if (DEBUG == true) {
        if (LogLevel == -1) {
            FullText += " Error Code: " + std::to_string(GetLastError());
        }
    }

    std::cout << FullText << std::endl;
    SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void logsnippet(int LogLevel, std::string FilePath, std::string Context, int Line, int Character) {
    std::string Spaces = std::string(" ", Character);

    log(LogLevel, "[ " + FilePath + " ] " + Context);
    std::cout << "> " + GetLineSnippet(FilePath, Line) << std::endl;
    std::cout << Spaces +  "  ^" << std::endl;
}