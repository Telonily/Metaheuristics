#pragma once

#include <vector>
#include <iostream>

class Individual {
private:
    int* tsp;
    int* kp;
    float fitness;

    int tspSize;
    int kpSize;

public:
    Individual(){}

    void setFitness(float fitness) {
        Individual::fitness = fitness;
    }

    float getFitness() const {
        return fitness;
    }

    Individual(int nodesCount, int itemsCount)
    {
        tsp = new int[nodesCount];
        kp = new int[itemsCount];

        tspSize = nodesCount;
        kpSize = itemsCount;
    }

    int *getTsp() const {
        return tsp;
    }

    int *getKp() const {
        return kp;
    }

    void swap(int index1, int index2)
    {
        int temp = tsp[index1];
        tsp[index1] = tsp[index2];
        tsp[index2] = temp;
    }

    virtual ~Individual() {
        delete(tsp);
        delete(kp);
    }

    Individual(const Individual& other)
    {
        tspSize = other.tspSize;
        kpSize = other.kpSize;

        tsp = new int[tspSize];
        int* otherTsp = other.getTsp();
        for (int i = 0; i < tspSize; i++)
        {
            tsp[i] = otherTsp[i];
        }

        kp = new int[kpSize];
        int* otherKp = other.getKp();
        for (int i = 0; i < kpSize; i++)
        {
            kp[i] = otherKp[i];
        }
    }

    Individual& operator=(const Individual& other)
    {
        tspSize = other.tspSize;
        kpSize = other.kpSize;

        tsp = new int[tspSize];
        int* otherTsp = other.getTsp();
        for (int i = 0; i < tspSize; i++)
        {
            tsp[i] = otherTsp[i];
        }

        kp = new int[kpSize];
        int* otherKp = other.getTsp();
        for (int i = 0; i < kpSize; i++)
        {
            kp[i] = otherKp[i];
        }
        return *this;
    }



    bool operator==(const Individual& other) const
    {
        int* otherTsp = other.getTsp();
        for (int i = 0; i < tspSize; i++)
        {
            if (tsp[i] != otherTsp[i]) return false;
        }

        int* otherKp = other.getTsp();
        for (int i = 0; i < kpSize; i++)
        {
            if (kp[i] != otherKp[i]) return false;
        }
        return true;
    }


    void printIndividual()
    {
        for (int i = 0; i < tspSize; i++)
        {
            printf("%d ", tsp[i]);
        }
        printf(" | ");

        for (int i = 0; i < kpSize; i++)
        {
            printf("%d ", kp[i]);
        }

        printf(" | ");
        printf("%.2f", getFitness());
        printf(" | %p", this);
    }
};


