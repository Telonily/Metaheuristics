#pragma once

#include "Item.h"

#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Node
{
private:
    int x, y;
    vector<Item> items;
public:
    Node(int x, int y);
    int getX() const;
    int getY() const;
    bool operator==(const Node& n) const;
    friend ostream& operator<<(ostream& os, const Node& n);
};