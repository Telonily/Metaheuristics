//
// Created by User on 10/8/2018.
//

#include "Node.h"

Node::Node(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Node::operator==(const Node& n) const
{
    return x == n.x && y == n.y;
}