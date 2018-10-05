#include "Loader.h"

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

		float dim = Loader::getParam(initStrings[2]);
		float itemsCount = Loader::getParam(initStrings[3]);
		float knapsackCapacity = Loader::getParam(initStrings[4]);
		float minSpeed = Loader::getParam(initStrings[5]);
		float maxSpeed = Loader::getParam(initStrings[6]);
		float rentRatio = Loader::getParam(initStrings[7]);

		printf("Dimensions: %.2f, Items: %.2f, Knapsack Capacity: %.2f, Min Speed: %.2f, Max Speed: %.2f, Renting ratio: %.2f", dim, itemsCount, knapsackCapacity,	minSpeed, maxSpeed, rentRatio);

		while (getline(in, line))
		{
			cout << line << endl;
		}
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

