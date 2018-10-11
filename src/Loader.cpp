#include "Loader.h"
#include "Node.h"
#include "Item.h"
#include "GADataContainer.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;

GADataContainer Loader::Load(string path)
{
    string line;
    GADataContainer data;
    ifstream in(path);
    if (in.is_open())
    {
        string initStrings[10] = {  };
        for (int i = 0; i < 9; i++)
        {
            getline(in, line);
            initStrings[i] = line;
            //printf ("%d\n", getParam(line));
        }


        int nodesCount = (int)Loader::getParam(initStrings[2]);
        int itemsCount = (int)Loader::getParam(initStrings[3]);
        int knapsackCapacity = (int)Loader::getParam(initStrings[4]);
        float minSpeed = Loader::getParam(initStrings[5]);
        float maxSpeed = Loader::getParam(initStrings[6]);
        float rentRatio = Loader::getParam(initStrings[7]);

        getline(in, line);
        vector<Node> nodes;
        for (int i = 0; i < nodesCount; i++)
        {
            getline(in, line);
            Node n = parseNode(line);
            nodes.push_back(n);
        }

        getline(in, line);
        vector<Item> items;
        for (int i = 0; i < itemsCount; i++)
        {
            getline(in, line);
            Item item = parseItem(line);
            items.push_back(item);
        }

        data.nodesCount = nodesCount;
        data.itemsCount = itemsCount;
        data.knapsackCapacity = knapsackCapacity;
        data.minSpeed = minSpeed;
        data.maxSpeed = maxSpeed;
        data.rentRatio = rentRatio;
        data.items = items;
        data.nodes = nodes;
    }
    else
    {
        printf("Could not load file");
    }
    in.close();
    return data;
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
    int id = parseFloat(splitString[0]);
    int x = parseFloat(splitString[1]);
    int y = parseFloat(splitString[2]);
    Node n(x, y);
    return n;
}

Item Loader::parseItem(string line)
{
    vector<string> splitString;
    boost::split(splitString, line, boost::is_any_of("\t"));
    int id = parseInt(splitString[0]);
    int profit = parseInt(splitString[1]);
    int weight = parseInt(splitString[2]);
    int assignedNode = parseInt(splitString[3]);
    Item item(profit, weight, assignedNode);
    return item;
}

int Loader::parseFloat(string s)
{
    float res = 0;
    try
    {
        res = boost::lexical_cast< float >( s );
    }
    catch( const boost::bad_lexical_cast & )
    {
        res = -1;
    }
    return res;
}

int Loader::parseInt(string s)
{
    int res = 0;
    try
    {
        res = boost::lexical_cast< int >( s );
    }
    catch( const boost::bad_lexical_cast & )
    {
        res = -1;
    }
    return res;
}