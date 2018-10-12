#include "GA.h"
#include "Loader.h"

#include <math.h>
#include <random>
#include <ctime>
#include <algorithm>

vector<vector<int>> pop;
vector<vector<int>> nextPop;


int popSize = 10;

void GA::loadData(string path) {
    data = Loader::Load(path);

    int nodesCount = data.nodes.size();
    distances = new float*[nodesCount];
    for(int i = 0; i < nodesCount; ++i)
        distances[i] = new float[nodesCount];

    for (int i = 0; i < nodesCount; i++)
    {
        for (int j = 0; i < nodesCount; i++)
        {
            distances[i][j] = GA::getDistance(data.nodes[i], data.nodes[j]);
        }
    }
}

float GA::calcFitness(Individual individual) {
    float res = 0;

    vector<int> genome = individual.tsp;

    for (int i = 0; i < genome.size()-1; i++)
    {
        res += *distances[genome[i], genome[i+1]];
    }
    res += *distances[genome[genome.size()-1], genome[0]];

    return res;
}

float GA::getDistance(Node n1, Node n2) {
    double x1 = n1.getX();
    double y1 = n1.getY();

    double x2 = n2.getX();
    double y2 = n2.getY();

    return sqrt (pow( (x2-x1), 2 ) + pow ( (y2-y1), 2 ));
}


void GA::start(int sec)
{

    Individual gen = this->generateGenome();
    for (auto i = gen.tsp.begin(); i < gen.tsp.end(); ++i)
    {
        cout << *i << endl;
    }

    cout << calcFitness(gen) << endl;

    time_t endTime = time(nullptr)+sec;
    while ( time(nullptr) < endTime )
    {

    }
}

auto rng = std::default_random_engine {};

Individual GA::generateGenome()
{
    int nodesCount = data.nodes.size();
    vector<int>genome(nodesCount);
    for (int i = 0; i < nodesCount; i++)
    {
        genome[i] = i+1;
    }

    std::shuffle(std::begin(genome)+1, std::end(genome), rng);

    Individual i(genome);

    return i;
}



