#pragma once

#include <string>
#include <iostream>
using namespace std;

class Node
{
private:
    int x, y;
public:
    Node(int x, int y);
    int getX() const;
    int getY() const;
    bool operator==(const Node& n) const;
    friend ostream& operator<<(ostream& os, const Node& n);

};