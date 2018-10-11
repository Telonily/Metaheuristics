#pragma once

#include "Node.h"

#include <vector>


class TspGA {
    vector<Node> nodes;
    float **distances;
    float fitness(vector<int> genome);
public:
    TspGA(const vector<Node> &nodes);
    void start(int sec);
    vector<int> generateGenome();
};


