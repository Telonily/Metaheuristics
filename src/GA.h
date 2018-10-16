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
    float calcFitness(Individual* i);
    float **distances;

    static float getDistance(Node n1, Node n2);
    void start(int sec);
    Individual generateGenome();
    void generateKnp(Individual* individual);
    void mutate(Individual *individual);
    void cross(Individual* ind1, Individual* ind2);
    Individual* select();


};
