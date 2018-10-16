#pragma once

#include <vector>

class Individual {
public:
    std::vector<int> tsp;
    std::vector<int> kp;
    float fitness;

    void setFitness(float fitness) {
        Individual::fitness = fitness;
    }

    Individual(const std::vector<int> &tsp) : tsp(tsp) {}
};


