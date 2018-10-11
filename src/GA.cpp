#include "GA.h"
#include "Loader.h"

#include <math.h>

void GA::loadData(string path) {
    data = Loader::Load(path);
}

float GA::fitness(Individual i) {
    return 0;
}

float GA::tspFitness(vector<int> tsp) {

    return 0;
}

float GA::getDistance(Node n1, Node n2) {
    double x1 = n1.getX();
    double y1 = n1.getY();

    double x2 = n2.getX();
    double y2 = n2.getY();

    return sqrt (pow( (x2-x1), 2 ) + pow ( (y2-y1), 2 ));
}


