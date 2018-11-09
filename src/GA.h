#pragma once

#include <vector>
#include "Node.h"
#include "Item.h"
#include "GADataContainer.h"
#include "Individual.h"

class GA {

public:
    GA();

    GADataContainer data;
    void loadData(string path);
    float calcFitness(Individual* i);
    float calcFitness2(Individual* i);
    float **distances;

    static float getDistance(Node n1, Node n2);
    void start(bool isTime, int count);
    Individual* generateGenome();
    void generateKnp(Individual* individual);
    void generateKnp2(Individual* individual);
    void mutate(Individual *individual);
    Individual* cross(Individual* ind1, Individual* ind2);
    int select();
    int getRandomInt(int lim1, int lim2);
    void showPop(vector<Individual*>* pop);
    void printIndividual(Individual* ind);
    float getRandomFloat(float lim1, float lim2);
    void startTabu(int tabuSize, int neighbours, int generations);
    Individual* getNeighbour(Individual* ind);
    void startAnealing(int neighbours, int generations);
};
