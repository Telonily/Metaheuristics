#include "Logger.h"

#include <ctime>
#include <fstream>


Logger::Logger(string baseName) {
    fileName = baseName + ' ' + to_string(time(nullptr))+".csv";
}

void Logger::writeLine(string line) {
    data.push_back(line);
}

bool Logger::save() {
    ofstream fileStream;
    fileStream.open(fileName);
    for (auto it = data.begin(); it < data.end(); it++)
    {
        fileStream << *it << endl;
    }
    fileStream.close();
}
