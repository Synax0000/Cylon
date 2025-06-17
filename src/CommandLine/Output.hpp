#include <iostream>
#include <string>

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

/*
-1 = ERROR
0 = WARNING
1 = INFO
2 = DEBUG
*/
void log(int Level,std::string Text);

/*
-1 = ERROR
0 = WARNING
1 = INFO
2 = DEBUG
*/
void logsnippet(int LogLevel, std::string FilePath, std::string Context, int Line, int Character);

#endif