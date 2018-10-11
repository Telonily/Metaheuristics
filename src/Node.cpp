//
// Created by User on 10/8/2018.
//

#include "Node.h"
#include <iostream>
using namespace std;

Node::Node(int x, int y) : x(x), y(y) {}

bool Node::operator==(const Node& n) const
{
    return x == n.x && y == n.y;
}

ostream& operator<<(ostream& os, const Node& n)
{
    os << "Node: x=" << n.x << ", y=" << n.y;
    return os;
}

int Node::getX() const {
    return x;
}

int Node::getY() const {
    return y;
}
