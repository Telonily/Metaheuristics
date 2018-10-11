#pragma once

#include "Node.h"
#include "Item.h"

#include <vector>

class GADataContainer {
public:
    int nodesCount, itemsCount, knapsackCapacity;
    float minSpeed, maxSpeed, rentRatio;
    vector<Node> nodes;
    vector<Item> items;

    GADataContainer() {}
};


