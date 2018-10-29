#include "TabuList.h"
#include "GA.h"


TabuList::TabuList(int size) : size(size) {
    list = new Individual*[size];

    for (int i = 0; i < size; i++)
    {
        list[i] = nullptr;
    }

    freeCell = 0;
}

void TabuList::add(Individual* ind) {
    list[freeCell] = ind;
    freeCell = (freeCell+1 == size) ? 0 : freeCell+1;
}

bool TabuList::contains(Individual *ind) {

    for (int i = 0; i < size; i++)
    {
        if (ind == list[i]) return true;

        if (list[i] != nullptr && *list[i] == *ind)
            return true;
    }
    return false;
}

void TabuList::print() {
    for (int i = 0; i < size; i++)
    {
        if (list[i] != nullptr)
        {
           list[i] -> printIndividual();
           printf("\n");
        }
    }
}

