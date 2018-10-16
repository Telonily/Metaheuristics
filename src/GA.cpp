#include "GA.h"
#include "Loader.h"

#include <math.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>

vector<Individual> pop;
vector<Individual> nextPop;
int popSize = 10;
int tournamentSize = 4;

vector<int> *locations;
float x; // (maxSpeed - minSpeed) / knapsackCapacity

std::mt19937 engine(time(0));

void GA::start(int sec)
{
    Individual ind1 = GA::generateGenome();
    Individual ind2 = GA::generateGenome();
    GA::cross(&ind1, &ind2);

/*
    Individual gen = GA::generateGenome();
    //GA::generateKnp(&gen);
    printf("Fitness: %.2f\n", GA::calcFitness(&gen));

    time_t endTime = time(nullptr)+sec;

    // Generate population and calculate fitness
    for (int i = 0; i < popSize; i++)
    {
        Individual ind = GA::generateGenome();
        GA::calcFitness(&ind);
        pop.push_back(ind);
    }

    //Starting GA
    while ( time(nullptr) < endTime )
    {

    }
*/
}
void GA::loadData(string path) {
    data = Loader::Load(path);
    int nodesCount = data.nodes.size();
    distances = new float*[nodesCount];
    for(int i = 0; i < nodesCount; i++)
        distances[i] = new float[nodesCount];

    for (int i = 0; i < nodesCount; i++)
    {
        for (int j = 0; j < nodesCount; j++)
        {
            float dis = GA::getDistance(data.nodes[i], data.nodes[j]);
            distances[i][j] = dis;
        }
    }

    locations = new vector<int>[data.nodesCount];

    for (int i = 0; i < data.nodesCount; i++)
    {
        for (int j = 0; j < data.itemsCount; j++)
        {
            if (data.items[j].getAssignedNode() == i)
            {
                locations[i].push_back(j);
            }
        }
    }

    x = (data.maxSpeed - data.minSpeed) / data.knapsackCapacity;

    // Print each node and items in there
/*

    for (int i = 0; i < data.nodesCount; i++)
    {
        printf("Node #%d: ", i);
        for (int j = 0; j < locations[i].size(); j++)
        {
            cout << locations[i][j] << " ";
        }
        cout << endl;
    }
*/

}

float GA::calcFitness(Individual* individual) {

    vector<int> route = individual->tsp;
    GA::generateKnp(individual);
    vector<int> knp = individual->kp;
    float speed = 1;
    float kWeight = 0;
    float tripTime = 0;

    for (int i = 0; i < route.size(); i++)
    {
        int currentNode = route[i];
        vector<int> itemsHere = locations[currentNode];
        for (int j = 0; j < itemsHere.size(); j++)
        {
            if (knp[itemsHere[j]] == currentNode)
            {
                speed = data.maxSpeed - (kWeight * x);
                int nextNode = (i == route.size()-1) ? route[0] : route[i+1];
                tripTime += distances[currentNode][nextNode] / speed;
            }
        }
    }

    float profit = data.rentRatio * tripTime;
    individual->fitness = profit;
    return profit;
}

float GA::getDistance(Node n1, Node n2) {
    double x1 = n1.getX();
    double y1 = n1.getY();

    double x2 = n2.getX();
    double y2 = n2.getY();

    return sqrt (pow( (x2-x1), 2 ) + pow ( (y2-y1), 2 ));
}

Individual GA::generateGenome()
{
    int nodesCount = data.nodes.size();
    vector<int>genome(nodesCount);
    for (int i = 0; i < nodesCount; i++)
    {
        genome[i] = i;
    }
    std::shuffle(std::begin(genome), std::end(genome), engine);
    Individual i(genome);
    return i;
}

//TODO: Make this more clever
void GA::generateKnp(Individual* individual) {
    vector<int> knp(data.itemsCount);
    int limit = data.knapsackCapacity;
    int currentWeight = 0;
    for (int i = 0; i < data.itemsCount; i++)
    {
        Item item = data.items[i];
        if ( currentWeight + item.getWeight() > limit)
        {
            knp[i] = -1;
        } else /*if (rand()%2 == 1)*/
        {
            knp[i] = data.items[i].getAssignedNode();
            currentWeight += item.getWeight();
        }
    }

    individual->kp = knp;

/*

    for (int i = 0; i < data.itemsCount; i++)
    {
        cout << knp[i] << " ";
    }
*/


}

void GA::mutate(Individual *individual) {
    int nodesCount = data.nodesCount;
    int index1 = rand() % nodesCount;
    int index2 = rand() % nodesCount;
    auto beginIter = individual->tsp.begin();
    iter_swap(beginIter+index1, beginIter+index2);
}

// OX crossing operator
void GA::cross(Individual* ind1, Individual* ind2)
{
    int child1[data.nodesCount];
    int child2[data.nodesCount];
    int half = data.nodesCount/2;
    int index1 = getRandom(1, half);
    int index2 = getRandom(half, data.nodesCount-2);

    printf("index1: %d index2: %d\n", index1, index2);

    bool used[data.nodesCount];
    fill_n(used, data.nodesCount, 0);

    for (int i = index1; i < index2; i++)
    {
        int t = ind2->tsp[i];
        child1[i] = t;
        used[t] = true;
    }

    int currentPos = index2;
    for (int i = index2; i < data.nodesCount;)
    {
        currentPos = (currentPos == data.nodesCount) ? 0 : currentPos;
        if ( !used[ind1->tsp[currentPos]] )
        {
            child1[i] = ind1->tsp[currentPos];
            i++;
        }
        currentPos++;
    }

    for (int i = 0; i < index1;)
    {
        currentPos = (currentPos == data.nodesCount) ? 0 : currentPos;
        if ( !used[ind1->tsp[currentPos]] )
        {
            child1[i] = ind1->tsp[currentPos];
            i++;
        }
        currentPos++;
    }



    for (int i = 0; i < data.nodesCount; i++)
    {
        printf("%d ", ind1->tsp[i]);
    }
    printf("\n");

    for (int i = 0; i < data.nodesCount; i++)
    {
        printf("%d ", ind2->tsp[i]);
    }
    printf("\n");

    for (int i = 0; i < data.nodesCount; i++)
    {
        printf("%d ", child1[i]);
    }
    printf("\n");

}

Individual* GA::select()
{
    int best = rand() % popSize;
    for (int i = 0; i < tournamentSize-1; i++)
    {
        if (pop[rand() % popSize].fitness > pop[best].fitness)
        {
            best = i;
        }
    }
    return &pop[best];
}

int GA::getRandom(int lim1, int lim2) {
    std::uniform_int_distribution<int> distribution(lim1, lim2);
    return distribution(engine);
}








