#pragma once

#include <vector>
#include "Node.h"
#include "Item.h"
#include "GADataContainer.h"
#include "Individual.h"

class GA {

public:
    GADataContainer data;
    void loadData(string path);
    float fitness(Individual i);
    float tspFitness(vector<int> tsp);
    static float getDistance(Node n1, Node n2);
};
