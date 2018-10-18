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
    Individual* generateGenome();
    void generateKnp(Individual* individual);
    void mutate(Individual *individual);
    Individual* cross(Individual* ind1, Individual* ind2);
    Individual* select();
    int getRandomInt(int lim1, int lim2);
    void showPop(vector<Individual*>* pop);
    void printIndividual(Individual* ind);
    float getRandomFloat(float lim1, float lim2);
};
