#pragma once

#include "Individual.h"

class TabuList {
private:

    int size;
    int freeCell;
    Individual** list;

public:
    explicit TabuList(int size);
    void add(Individual* ind);
    bool contains(Individual* ind);
    void print();
};


