//
// Created by User on 10/8/2018.
//

#ifndef METAHEURISTICS_NODE_H
#define METAHEURISTICS_NODE_H


class Node
{
private:
    int x, y;
public:
    Node(int x, int y);
    int getX() { return x; }
    int getY() { return y; }
    bool operator==(const Node& n) const;
};


#endif //METAHEURISTICS_NODE_H
