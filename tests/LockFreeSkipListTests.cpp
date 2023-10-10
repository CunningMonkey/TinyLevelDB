#include <gtest/gtest.h>
#include "../src/LockFreeSkipList.h" // Make sure to adjust the path accordingly

TEST(LockFreeSkipListTests, InsertTest)
{
    LockFreeSkipList<int, int> skipList;
    ASSERT_TRUE(skipList.insert(5, 5));
    // Add more assertions...
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
