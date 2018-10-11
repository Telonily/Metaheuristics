#include <string>
#include "Node.h"

using namespace std;


#ifndef METAHEURISTICS_LOADER_H
#define METAHEURISTICS_LOADER_H


class Loader {
public:
    static void Load(string path);
    static float getParam(string line);
    static Node parseNode(string line);
};


#endif //METAHEURISTICS_LOADER_H









