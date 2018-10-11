#pragma once

#include <string>
#include "Node.h"
#include "Item.h"
#include "GADataContainer.h"

using namespace std;

class Loader {
public:
    static GADataContainer Load(string path);
    static float getParam(string line);
    static Node parseNode(string line);
    static int parseFloat(string s);
    static int parseInt(string s);
    static Item parseItem(string line);
};