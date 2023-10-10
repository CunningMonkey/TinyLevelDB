#include <gtest/gtest.h>
#include "../src/LockFreeSkipList.h" // Make sure to adjust the path accordingly

class Comparator {
    int operator()(const int& a, const int& b) const {
        if (a < b) {
            return -1;
        } else if (a == b) {
            return 0;
        } else {
            return 1;
        }
    }
};

TEST(LockFreeSkipListTests, InsertTest)
{
    LockFreeSkipList<int, int, Comparator> skipList;
    ASSERT_TRUE(skipList.put(5, 5));
    // Add more assertions...
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
