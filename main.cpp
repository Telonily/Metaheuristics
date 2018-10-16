#include "src/Loader.h"
#include "src/Node.h"
#include "src/GA.h"

#include <iostream>
#include <string>
#include <math.h>
#include <random>

#include <gtest/gtest.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    //testing::InitGoogleTest(&argc, argv);
    //RUN_ALL_TESTS();


    GA ga;
    ga.loadData("../ttp_student/trivial_0.ttp");
//    ga.loadData("../ttp_student/hard_2.ttp");
    printf("Dimensions: %d, Items: %d, Knapsack Capacity: %d, Min Speed: %.2f, Max Speed: %.2f, Renting ratio: %.2f\n",
            ga.data.nodesCount, ga.data.itemsCount, ga.data.knapsackCapacity, ga.data.minSpeed, ga.data.maxSpeed, ga.data.rentRatio);
    ga.start(3);
    return 1;
}






TEST(Tests, Test) {
    Node n(2,3);
    ASSERT_EQ(Loader::parseNode("1\t2\t3"), n);

    Node n1(-1,3);
    Node n2(6, 2);
    ASSERT_EQ(GA::getDistance(n1, n2), sqrt(2)*5);
}

TEST(BasicTest, ItemTest) {
    Item item(1992,1792,7);
    ASSERT_EQ(Loader::parseItem("6\t1992\t1792\t7"), item);
}

