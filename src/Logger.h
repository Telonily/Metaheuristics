#pragma once

#include <string>
#include <vector>

using namespace std;

class Logger {
private:
    string fileName;
    vector<string> data;

public:
    explicit Logger(string baseName);
    void writeLine(string line);
    bool save();
};


