#include "src/Loader.h"
#include "src/Node.h"

#include <iostream>
#include <string>

#include <gtest/gtest.h>

using namespace std;

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    //Loader::Load("../ttp_student/easy_0.ttp");

    Node n(1, 10);
    printf("%d", n.getX());
    return 1;
}


TEST(BasicTest, TestName) {
    Node n(2,3);
    ASSERT_EQ(Loader::parseNode("1\t2\t3"), n);
    ASSERT_EQ(1,1);
}
