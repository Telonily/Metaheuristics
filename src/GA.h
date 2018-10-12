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
    float calcFitness(Individual i);
    float **distances;

    static float getDistance(Node n1, Node n2);
    void start(int sec);
    Individual generateGenome();

};
