#include "Output.hpp"

#include <iostream>
#include <string>

#include <Windows.h>

#define DEBUG // Temp for now, will be moved to another file when needed

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
            #ifndef DEBUG
                return;
            #endif

            SetConsoleTextAttribute(StandardHandle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
            FullText += "[DEBUG] ";
            break;
        default:
            SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "[Invalid Log Level Passed]" << std::endl;
            return;
    }

    FullText += Text + ".";

    #ifdef DEBUG
        if (LogLevel == -1) {
            FullText += " Error Code: " + std::to_string(GetLastError());
        }
    #endif

    std::cout << FullText << std::endl;
    SetConsoleTextAttribute(StandardHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}