#include "Loader.h"
#include "Node.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;

void Loader::Load(string path)
{
    string line;

    ifstream in(path);
    if (in.is_open())
    {
        vector<string> initStrings;
        for (int i = 0; i < 9; i++)
        {
            getline(in, line);
            initStrings.insert(initStrings.end(), line);
        }

        int nodeCount = (int)Loader::getParam(initStrings[2]);
        int itemsCount = (int)Loader::getParam(initStrings[3]);
        int knapsackCapacity = (int)Loader::getParam(initStrings[4]);
        float minSpeed = Loader::getParam(initStrings[5]);
        float maxSpeed = Loader::getParam(initStrings[6]);
        float rentRatio = Loader::getParam(initStrings[7]);

        printf("Dimensions: %.2f, Items: %.2f, Knapsack Capacity: %.2f, Min Speed: %.2f, Max Speed: %.2f, Renting ratio: %.2f\n", nodeCount, itemsCount, knapsackCapacity,	minSpeed, maxSpeed, rentRatio);

        getline(in, line);

        vector<Node> nodes;
        for (int i = 0; i < nodeCount; i++)
        {
            getline(in, line);
            Node n = parseNode(line);
            nodes.push_back(n);
        }

        while (getline(in, line))
        {
            cout << line << endl;
        }
    }
    else
    {
        printf("Could not load file");
    }
    in.close();
}

float Loader::getParam(string line)
{

    vector<string> strs;
    boost::split(strs, line, boost::is_any_of("\t"));

    float res;
    try
    {
        res = boost::lexical_cast< float >( strs[1] );
    }
    catch( const boost::bad_lexical_cast & )
    {
        res = -1;
    }
    return res ;
}

Node Loader::parseNode(string line)
{
    vector<string> splitString;
    boost::split(splitString, line, boost::is_any_of("\t"));
    int id = boost::lexical_cast< int >( splitString[0] );
    int x = boost::lexical_cast< int >( splitString[1] );
    int y = boost::lexical_cast< int >( splitString[2] );
    Node n(x, y);
    return n;
}