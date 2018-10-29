#include "src/Loader.h"
#include "src/Node.h"
#include "src/GA.h"
#include "src/TabuList.h"

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

    /*
    TabuList t(3);
    auto i1 = new Individual(1, 1);
    i1->getTsp()[0] = 99;
    i1->getKp()[0] = 99;
    auto i2 = new Individual(1, 1);
    i2->getTsp()[0] = 88;
    i2->getKp()[0] = 88;
    auto i3 = new Individual(1, 1);
    i3->getTsp()[0] = 77;
    i3->getKp()[0] = 77;
    auto i4 = new Individual(1, 1);
    i4->getTsp()[0] = 66;
    i4->getKp()[0] = 66;

    t.add(i1);
    t.add(i2);
    t.add(i3);
    t.add(i4);

    bool exist3 = t.contains(i3);
    bool exist1 = t.contains(i1);
*/


    GA ga;
    ga.start(false, 100);
    ga.startTabu(100, 5, 100);

    return 1;
}




/*

TEST(Tests, Test) {
    Node n(2,3);
    ASSERT_EQ(Loader::parseNode("1\t2\t3"), n);

    Node n1(-1,3);
    Node n2(6, 2);
    ASSERT_EQ(GA::getDistance(n1, n2), sqrt(2)*5);
}

TEST(BasicTest, ItemTest) {
    Item item(1992,1792,7,0);
    ASSERT_EQ(Loader::parseItem("6\t1992\t1792\t7"), item);
}

*/