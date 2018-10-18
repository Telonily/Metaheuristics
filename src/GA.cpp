#include "GA.h"
#include "Loader.h"

#include <math.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>

vector<Individual*> pop;
vector<Individual*> nextPop;
int popSize = 100;
int tournamentSize = 2;
float proportion = 0.45;
float mutation_chance = 0.1;

vector<int> *locations;
float x; // (maxSpeed - minSpeed) / knapsackCapacity

std::mt19937 engine(time(0));

//#define DEBUG

void GA::start(int sec)
{
    time_t endTime = time(nullptr)+sec;

    // Generate population and calculate fitness
    for (int i = 0; i < popSize; i++)
    {
        Individual* ind = GA::generateGenome();
        calcFitness(ind);
        pop.push_back(ind);
    }

    while ( time(nullptr) < endTime )
    {
        // Select in new population
        for (int i = 0; i < int(popSize*proportion); i++)
        {
            Individual* ind = select();

            #ifdef DEBUG
            printf("SELECTED: ");
            printIndividual(ind);
            printf("\n");
            #endif

            mutate(ind);
            calcFitness(ind);
            nextPop.push_back(ind);
        }

        // Cross for new pop while != full
        int limit = popSize-nextPop.size();
        for (int i = 0; i < limit; i++)
        {
            Individual* ii1 = select();
            Individual* ii2;
            do {ii2 = select();} while (ii1 == ii2);
            Individual* iii = cross(ii1, ii2);
            mutate(iii);
            calcFitness(iii);
            nextPop.push_back(iii);
        }

        // Revert pop, nextPop. Clear nextPop
        pop.clear();
        pop.assign(nextPop.begin(), nextPop.end());
        nextPop.erase(nextPop.begin(), nextPop.end());

        #ifdef DEBUG
        printf("==========POP==========\n");
        showPop(&pop);
        #endif

        //Calculate stats
        Individual* best = pop[0];
        Individual* worst = pop[0];
        float average = 0;
        average += pop[0]->fitness;

        for (auto iter = pop.begin()+1; iter < pop.end(); iter++)
        {
            Individual* ind = *iter;
            if (ind->fitness < best->fitness) {best = ind;}
            if (ind->fitness > worst->fitness) {worst = ind;}
            average += ind->fitness;
        }

        printf("%.2f %.2f %.2f\n", best->fitness, worst->fitness, average/pop.size());
    }

}

void GA::loadData(string path)
{
    data = Loader::Load(path);

    // Pre calculate distances between nodes
    distances = new float*[data.nodesCount];
    for(int i = 0; i < data.nodesCount; i++)
        distances[i] = new float[data.nodesCount];

    for (int i = 0; i < data.nodesCount; i++)
    {
        for (int j = 0; j < data.nodesCount; j++)
        {
            float dis = GA::getDistance(data.nodes[i], data.nodes[j]);
            distances[i][j] = dis;
        }
    }

    // Index is number of node, value is vector of items
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
}

float GA::calcFitness(Individual* individual)
{
    generateKnp(individual);

    vector<int> route = *individual->tsp;
    vector<int> knp = *individual->kp;

    float speed = 1;
    float weight = 0;
    float tripTime = 0;

    for (int i = 0; i < route.size(); i++)
    {
        int currentNode = route[i];
        vector<int> itemsHere = locations[currentNode];
        for (int j = 0; j < itemsHere.size(); j++)
        {
            if (knp[itemsHere[j]] == currentNode)
            {
                speed = data.maxSpeed - (weight * x);
                int nextNode = (i == route.size()-1) ? route[0] : route[i+1];
                tripTime += distances[currentNode][nextNode] / speed;
            }
        }
    }

    float profit = data.rentRatio * tripTime;
    individual->fitness = profit;
    return profit;
}

float GA::getDistance(Node n1, Node n2)
{
    double x1 = n1.getX();
    double y1 = n1.getY();
    double x2 = n2.getX();
    double y2 = n2.getY();
    return sqrt (pow( (x2-x1), 2 ) + pow ( (y2-y1), 2 ));
}

Individual* GA::generateGenome()
{
    auto genome = new vector<int>();
    for (int i = 0; i < data.nodesCount; i++)
    {
        genome->push_back(i);
    }
    std::shuffle(std::begin(*genome), std::end(*genome), engine);
    auto i = new Individual(genome);
    return i;
}

//TODO: Make this more clever
void GA::generateKnp(Individual* individual)
{
    vector<int>* knp = new vector<int>(data.itemsCount);
    int limit = data.knapsackCapacity;
    int currentWeight = 0;
    for (int i = 0; i < data.itemsCount; i++)
    {
        Item item = data.items[i];
        if ( currentWeight + item.getWeight() > limit)
        {
            knp->at(i) = -1;
        } else /*if (rand()%2 == 1)*/
        {
            knp->at(i) = data.items[i].getAssignedNode();
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

void GA::mutate(Individual *individual)
{
    if (getRandomFloat(0, 1) < mutation_chance)
    {
        int index1 = getRandomInt(0, data.nodesCount - 1);
        int index2 = getRandomInt(0, data.nodesCount - 1);
        auto beginIter = individual->tsp->begin();
        iter_swap(beginIter+index1, beginIter+index2);
    }
}

// OX crossing operator
Individual* GA::cross(Individual* ind1, Individual* ind2)
{
    int child1[data.nodesCount];
    int half = data.nodesCount/2;
    int index1 = getRandomInt(1, half);
    int index2 = getRandomInt(half, data.nodesCount - 2);
    bool used[data.nodesCount];
    fill_n(used, data.nodesCount, 0);

    for (int i = index1; i < index2; i++)
    {
        int t = ind2->tsp->at(i);
        child1[i] = t;
        used[t] = true;
    }

    int currentPos = index2;
    for (int i = index2; i < data.nodesCount;)
    {
        currentPos = (currentPos == data.nodesCount) ? 0 : currentPos;
        int o = ind1->tsp->at(currentPos);
        if ( !used[o] )
        {
            child1[i] = ind1->tsp->at(currentPos);
            i++;
        }
        currentPos++;
    }

    for (int i = 0; i < index1;)
    {
        currentPos = (currentPos == data.nodesCount) ? 0 : currentPos;
        if ( !used[ind1->tsp->at(currentPos)] )
        {
            child1[i] = ind1->tsp->at(currentPos);
            i++;
        }
        currentPos++;
    }

    vector<int>* res = new vector<int> (child1, child1+( sizeof(child1) / sizeof(child1[0])) );
    auto ind = new Individual(res);
    return ind;
}

Individual* GA::select()
{
    int best = getRandomInt(0, popSize - 1);
    for (int i = 0; i < tournamentSize-1; i++)
    {
        int rnd = getRandomInt(0, popSize - 1);
        if (pop[rnd]->fitness < pop[best]->fitness)
        {
            best = rnd;
        }
    }
    return pop[best];
}


// HELPER FUNCTIONS

int GA::getRandomInt(int lim1, int lim2)
{
    std::uniform_int_distribution<int> distribution(lim1, lim2);
    return distribution(engine);
}

float GA::getRandomFloat(float lim1, float lim2)
{
    std::uniform_real_distribution<> distribution(lim1, lim2);
    return float(distribution(engine));
}

void GA::showPop(vector<Individual*> *pop)
{
    for (auto i = pop->begin(); i < pop->end(); i++)
    {
        auto ind = *i;
        printIndividual(ind);
        printf("\n");
    }
}

void GA::printIndividual(Individual *ind)
{
    for (auto it = ind->tsp->begin(); it < ind->tsp->end(); it++)
    {
        printf("%d ", *it);
    }
    printf(" | ");

    for (auto it = ind->kp->begin(); it < ind->kp->end(); it++)
    {
        printf("%d ", *it);
    }

    printf(" | ");
    printf("%.2f", ind->fitness);
    printf(" | %p", ind);
}






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




/* for debug
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

*/