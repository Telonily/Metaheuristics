#include "GA.h"
#include "Loader.h"
#include "TabuList.h"
#include "Logger.h"

#include <math.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>
#include <fstream>
#include <queue>
#include <list>
#include <boost/format.hpp>

string file = "medium_2";

vector<Individual*> pop;
vector<Individual*> nextPop;
int popSize = 100;
int tournamentSize = 3;
float proportion = 0.4;
float mutation_chance = 0.05;


vector<int> locationsBest;
vector<int> nonAcceptableItems;

vector<int> *locations;
float x; // (maxSpeed - minSpeed) / knapsackCapacity

std::mt19937 engine(time(0));

//#define DEBUG


void GA::start(bool isTime, int limitation)
{
    string fileName = file+" GA "+to_string(popSize)+" "+to_string(tournamentSize)+" "+to_string(proportion)+" "+to_string(mutation_chance);
    Logger logger(fileName);

    time_t endTime = time(nullptr)+limitation;


    vector<bool> selected(popSize);

    // Generate population and calculate fitness
    for (int i = 0; i < popSize; i++)
    {
        Individual* ind = GA::generateGenome();
        calcFitness(ind);
        pop.push_back(ind);
    }

    int generationCount = 0;
    while (time(nullptr) < endTime)
    {
        // Select in new population
        for (int i = 0; i < int(popSize*proportion); i++)
        {
            int selectedIndex = select();
            Individual* ind = pop[selectedIndex];
            selected[selectedIndex] = true;

            #ifdef DEBUG
            printf("SELECTED: ");
            printIndividual(ind);
            printf("\n");
            #endif

            nextPop.push_back(ind);
        }

        // Cross for new pop while != full
        int limit = popSize-nextPop.size();
        for (int i = 0; i < limit; i++)
        {
            Individual* ii1 = pop[select()];
            Individual* ii2;
            do {ii2 = pop[select()];} while (ii1 == ii2);
            Individual* iii = cross(ii1, ii2);
            calcFitness(iii);
            nextPop.push_back(iii);
        }

        for (int i = 0; i < popSize; i++)
        {
            if (getRandomFloat(0, 1) < mutation_chance)
            {
                mutate(nextPop[i]);
            }
            calcFitness(nextPop[i]);
        }

        // Revert pop, nextPop. Clear nextPop
        /*for (int i = 0; i < popSize; i++)
        {
            if (!selected[i])
            {
                delete(pop[i]);
            }
        }*/
        pop.clear();
        pop.assign(nextPop.begin(), nextPop.end());

        for (int i = 0; i < popSize; i++)
        {
            pop.push_back(nextPop.at(i));
        }

        nextPop.erase(nextPop.begin(), nextPop.end());

        #ifdef DEBUG
        printf("==========POP==========\n");
        showPop(&pop);
        #endif

        //Calculate stats
        Individual* best = pop[0];
        Individual* worst = pop[0];
        float average = 0;
        average += pop[0]->getFitness();

        for (auto iter = pop.begin()+1; iter < pop.end(); iter++)
        {
            Individual* ind = *iter;
            if (ind->getFitness() < best->getFitness()) {best = ind;}
            if (ind->getFitness() > worst->getFitness()) {worst = ind;}
            average += ind->getFitness();
        }

        //string output = boost::format("%.2f,%.2f,%.2f\n", best->getFitness(), worst->getFitness(), average/pop.size());
        string output = to_string(int(best->getFitness()))+","+to_string(int(worst->getFitness()))+","+to_string(int(average/pop.size()));
        cout << output << endl;
        logger.writeLine(output);
        //printIndividual(best);

        if (!isTime && ++generationCount == limitation )
        {
            //printIndividual(best);
            break;
        }



    }

    logger.save();
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

    locationsBest = vector<int>(data.nodesCount);
    fill_n(locationsBest.begin(), locationsBest.size(), -1);

    for (int i = 0; i < data.nodesCount; i++)
    {
        if (!locations[i].empty())
        {
            vector<int> itemsHere = locations[i];
            int bestItemId = itemsHere[0];
            for (auto iter = itemsHere.begin()+1; iter < itemsHere.end(); iter++)
            {
                if (data.items[*iter].getRatio() > data.items[bestItemId].getRatio())
                {
                    bestItemId = *iter;
                }
            }
            locationsBest.at(i) = bestItemId;
        }
    }

    vector<Item> bestItems;
    for (int i = 0; i < locationsBest.size(); i++)
    {
        int itemIndex = locationsBest[i];
        if (itemIndex > -1)
        {
            bestItems.push_back(data.items[itemIndex]);
        }
    }

    sort(bestItems.begin(), bestItems.end());


    for (int i = int(bestItems.size()*0.8); i < bestItems.size(); i++)
    {
        nonAcceptableItems.push_back(bestItems[i].getId());
    }

    /*
    for (auto it = nonAcceptableItems.begin(); it < nonAcceptableItems.end(); it++)
    {
        cout << data.items[*it] << endl;
    }
     */
}

float GA::calcFitness2(Individual* individual)
{
    generateKnp(individual);
    float fitness = 0;
    int* route = individual->getTsp();
    for (int i = 0; i < data.nodesCount-1; i++)
    {
        fitness += distances[route[i]][route[i+1]];
    }
    fitness += distances[route[data.nodesCount-1]][route[0]];
    individual->setFitness(fitness);
    return fitness;
}

//TODO: Repair
float GA::calcFitness(Individual* individual)
{
    generateKnp(individual);
    int* route = individual->getTsp();
    int* knp = individual->getKp();

    float speed;
    float weight = 0;
    float tripTime = 0;
    float value = 0;

    for (int i = 0; i < data.nodesCount; i++)
    {
        int currentNode = route[i];
        vector<int> itemsHere = locations[currentNode];
        for (int j = 0; j < itemsHere.size(); j++)
        {
            if (knp[itemsHere[j]] == currentNode)
            {
                int itemId = itemsHere[j];
                speed = data.maxSpeed - (weight * x);
                int nextNode = (i == data.nodesCount-1) ? route[0] : route[i+1];
                tripTime += distances[currentNode][nextNode] / speed;
                float currentValue = data.items[itemId].getProfit();
                value += currentValue;
            }
        }
    }

    float cost = data.rentRatio * tripTime;
    float fitness = cost-value;
    individual->setFitness(fitness);
    //printf("Fitness: %.2f\n", fitness);

    return fitness;
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
    auto ind = new Individual(data.nodesCount, data.itemsCount);
    auto genome = ind->getTsp();
    for (int i = 0; i < data.nodesCount; i++)
    {
        genome[i] = i;
    }
    shuffle(&genome[0], &genome[data.nodesCount], engine);
    return ind;
}

void GA::generateKnp(Individual* individual)
{
    /*
    vector<int>* knp = new vector<int>(data.itemsCount);
    int limit = data.knapsackCapacity;
    int currentWeight = 0;
    for (int i = 0; i < data.itemsCount; i++)
    {
        Item item = data.items[i];
        if ( currentWeight + item.getWeight() > limit)
        {
            knp->at(i) = -1;
        } else
        {
            knp->at(i) = data.items[i].getAssignedNode();
            currentWeight += item.getWeight();
        }
    }
    individual->kp = knp;
*/
    int* knp = individual->getKp();
    fill(&knp[0], &knp[data.itemsCount], -1);
    int* route = individual->getTsp();
    int limit = data.knapsackCapacity;
    int currentWeight = 0;
    for (int i = data.nodesCount-1; i > 0; i--)
    {
        int node = route[i];
        if (locationsBest[node] != -1)
        {
            Item item = data.items[locationsBest[node]];
            int itemId = item.getId();
            if ( currentWeight + item.getWeight() > limit)
            {
                knp[itemId] = -1;
            } else
            {
                knp[itemId] = item.getAssignedNode();
                currentWeight += item.getWeight();
            }
        }
    }
}

void GA::generateKnp2(Individual* individual)
{
/*
    vector<int>* knp = new vector<int>(data.itemsCount);
    int limit = data.knapsackCapacity;
    int currentWeight = 0;
    for (int i = 0; i < data.itemsCount; i++)
    {
        Item item = data.items[i];
        if ( currentWeight + item.getWeight() > limit)
        {
            knp->at(i) = -1;
        } else
        {
            knp->at(i) = data.items[i].getAssignedNode();
            currentWeight += item.getWeight();
        }
    }

    individual->kp = knp;
*/


    int* knp = individual->getKp();
    fill(&knp[0], &knp[data.itemsCount], -1);
    int maxWeight = data.knapsackCapacity;
    int currentWeight = 0;
    int* route = individual->getTsp();
    for (int i = data.nodesCount-1; i > 0; i--)
    {
        int currentNode = route[i];
        int possibleItemId = locationsBest[currentNode];
        int itemWeight = data.items[possibleItemId].getWeight();
        if(possibleItemId < 0 || itemWeight+currentWeight>maxWeight ||  find(nonAcceptableItems.begin(), nonAcceptableItems.end(), possibleItemId) != nonAcceptableItems.end()) {

        } else {
            knp[possibleItemId] = currentNode;
            currentWeight += itemWeight;
        }
    }
}

void GA::mutate(Individual *individual)
{
    int index1 = getRandomInt(0, data.nodesCount - 1);
    int index2 = getRandomInt(0, data.nodesCount - 1);
    individual->swap(index1, index2);
}

// OX crossing operator
Individual* GA::cross(Individual* ind1, Individual* ind2)
{
    auto ind = new Individual(data.nodesCount, data.itemsCount);
    int* child1 = ind->getTsp();
    int half = data.nodesCount/2;
    int index1 = getRandomInt(1, half);
    int index2 = getRandomInt(half, data.nodesCount - 2);
    bool used[data.nodesCount];
    fill_n(used, data.nodesCount, 0);

    int* ind1tsp = ind1->getTsp();
    int* ind2tsp = ind2->getTsp();


    for (int i = index1; i < index2; i++)
    {
        int t = ind2tsp[i];
        child1[i] = t;
        used[t] = true;
    }

    int currentPos = index2;
    for (int i = index2; i < data.nodesCount;)
    {
        currentPos = (currentPos == data.nodesCount) ? 0 : currentPos;
        if ( !used[ind1tsp[currentPos]] )
        {
            child1[i] = ind1tsp[currentPos];
            i++;
        }
        currentPos++;
    }

    for (int i = 0; i < index1;)
    {
        currentPos = (currentPos == data.nodesCount) ? 0 : currentPos;
        if ( !used[ind1tsp[currentPos]] )
        {
            child1[i] = ind1tsp[currentPos];
            i++;
        }
        currentPos++;
    }

    return ind;
}

int GA::select()
{
    int best = getRandomInt(0, popSize - 1);
    for (int i = 0; i < tournamentSize-1; i++)
    {
        int rnd = getRandomInt(0, popSize - 1);
        if (pop[rnd]->getFitness() < pop[best]->getFitness())
        {
            best = rnd;
        }
    }
    return best;
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
    int* tsp = ind->getTsp();
    for (int i = 0; i < data.nodesCount; i++)
    {
        printf("%d ", tsp[i]);
    }
    printf(" | ");

    int* knp = ind->getKp();
    for (int i = 0; i < data.itemsCount; i++)
    {
        printf("%d ", knp[i]);
    }

    printf(" | ");
    printf("%.2f", ind->getFitness());
    printf(" | %p", ind);
}

void GA::startTabu(int tabuSize, int neighbours, int generations) {
    TabuList tabuList(tabuSize);
    Individual* current = generateGenome();
    calcFitness(current);
    Individual* best = current;

    for (int i = 0; i < generations; i++)
    {
        for (int jj = 0; jj < neighbours; jj++)
        {
            Individual* neighbour = getNeighbour(current);

            if (!tabuList.contains(neighbour) && neighbour->getFitness() < current->getFitness())
            {
                current = neighbour;
            }
        }

        if (current->getFitness() < best->getFitness())
            best = current;

        if (!tabuList.contains(current))
            tabuList.add(current);

        printf("%.2f\n", best->getFitness());

        /*
        printf("TABU ITER %d\n", i);
        tabuList.print();
        printf("\n");
         */
    }


}

Individual* GA::getNeighbour(Individual* ind) {
    auto res = new Individual(*ind);
    mutate(res);
    calcFitness(res);
    return res;
}




GA::GA() {
    loadData("../ttp_student/"+file+".ttp");

    printf("Dimensions: %d, Items: %d, Knapsack Capacity: %d, Min Speed: %.2f, Max Speed: %.2f, Renting ratio: %.2f\n",
           data.nodesCount, data.itemsCount, data.knapsackCapacity, data.minSpeed, data.maxSpeed, data.rentRatio);
}

void GA::startAnealing(int neighbours, int generations) {
    string fileName = file+" Annealing "+to_string(neighbours);
    Logger logger(fileName);

    Individual* current = generateGenome();
    Individual* best = current;
    calcFitness(current);
    float temp = 100000;

    while (temp > 0.5)
    {
        for (int jj = 0; jj < neighbours; jj++)
        {
            Individual* neighbour = getNeighbour(current);

            if (neighbour->getFitness() < current->getFitness())
            {
                current = neighbour;
            } else
            {
                float difference = neighbour->getFitness() - current->getFitness();
                //double probability = 1/(1 + exp( (difference / temp) ));
                double probability = (exp( -(difference / temp) ));
                float random = getRandomFloat(0,1);

                /*
                cout << "Probab " << probability;
                printf(" neigh: %.2f curr %.2f ",neighbour->getFitness(), current->getFitness());
                printf(" diff %.2f temp %.2f \n", difference, temp);
                */
                if (random < probability)
                    current = neighbour;
            }

            temp *= 0.995;
            if (current->getFitness() < best->getFitness())
            {
                best = current;
            }

            //printf("%.2f\t%.2f\n", current->getFitness(), temp);
            string log = to_string(current->getFitness())+','+ to_string(best->getFitness()) +','+to_string(temp);
            logger.writeLine(log);
        }
    }


    logger.save();
}



// todo ARRAY CIRCLE QUEUE








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