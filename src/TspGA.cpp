#include "TspGA.h"
#include "GA.h"

#include <ctime>
#include <algorithm>
#include <random>

using namespace std;



void TspGA::start(int sec)
{

    vector<int> gen = this->generateGenome();
    for (auto i = gen.begin(); i < gen.end(); ++i)
    {
        cout << *i << endl;
    }

    cout << fitness(gen) << endl;

    time_t endTime = time(nullptr)+sec;
    while ( time(nullptr) < endTime )
    {

    }



}

auto rng = std::default_random_engine {};

vector<int> TspGA::generateGenome()
{
    int nodesCount = nodes.size();
    vector<int>genome(nodesCount);
    for (int i = 0; i < nodesCount; i++)
    {
        genome[i] = i+1;
    }

    std::shuffle(std::begin(genome)+1, std::end(genome), rng);
    return genome;
}


float TspGA::fitness(vector<int> genome)
{
    float res = 0;
    for (int i = 0; i < genome.size()-1; i++)
    {
        res += *distances[genome[i], genome[i+1]];
    }
    res += *distances[genome[genome.size()-1], genome[0]];
    return res;
}


TspGA::TspGA(const vector<Node> &nodes) : nodes(nodes)
{
    int nodesCount = nodes.size();
    distances = new float*[nodesCount];
    for(int i = 0; i < nodesCount; ++i)
        distances[i] = new float[nodesCount];

    for (int i = 0; i < nodesCount; i++)
    {
        for (int j = 0; i < nodesCount; i++)
        {
            distances[i][j] = GA::getDistance(nodes[i], nodes[j]);
        }
    }
}


