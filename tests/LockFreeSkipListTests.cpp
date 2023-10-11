#include "../src/LockFreeSkipList.h" // Assuming your class is saved in this header file
#include <gtest/gtest.h>

typedef uint64_t Key;

struct Comparator
{
    int operator()(const int &a, const int &b) const
    {
        if (a < b)
        {
            return -1;
        }
        else if (a > b)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};

int random_int(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

TEST(LockFreeSkipListTest, PutAndGet)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // Inserting a value
    skipList.put(1, 100);
    int *val = skipList.get(1);

    // Check if we get the correct value
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 100);
}

TEST(LockFreeSkipListTest, PutAndRemove)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // Inserting a value
    skipList.put(2, 200);
    bool removed = skipList.remove(2);
    int *val = skipList.get(2);

    // Check if the value was removed correctly
    EXPECT_TRUE(removed);
    EXPECT_EQ(val, nullptr);
}

TEST(LockFreeSkipListTest, RemoveNonExistent)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // Try removing a non-existent value
    bool removed = skipList.remove(3);
    int *val = skipList.get(3);

    EXPECT_FALSE(removed);
    EXPECT_EQ(val, nullptr);
}

TEST(LockFreeSkipListTest, MultipleInsertAndRetrieve)
{
    LockFreeSkipList<int, int, Comparator> skipList;
    const int N = 100;

    // 插入多个值
    for (int i = 0; i < N; i++)
    {
        skipList.put(i, i * 100);
    }

    // 检查是否可以正确检索每个值
    for (int i = 0; i < N; i++)
    {
        int *val = skipList.get(i);
        ASSERT_NE(val, nullptr);
        EXPECT_EQ(*val, i * 100);
    }
}

TEST(LockFreeSkipListTest, RandomOperations) {
    LockFreeSkipList<int, int, Comparator> skipList;
    std::map<int, int> reference; // 用于参考的标准map

    for (int i = 0; i < 1000; ++i) {
        int key = random_int(1, 1000);
        int value = random_int(1, 1000);

        skipList.put(key, value);
        reference[key] = value;

        // 随机选择是否删除一个键
        if (random_int(0, 1)) {
            int key_to_remove = random_int(1, 1000);
            skipList.remove(key_to_remove);
            reference.erase(key_to_remove);
        }
    }

    for (const auto& pair : reference) {
        ASSERT_EQ(*skipList.get(pair.first), pair.second);
    }
}

TEST(LockFreeSkipListTest, BoundaryConditions) {
    LockFreeSkipList<int, int, Comparator> skipList;

    // 测试空跳表的获取和删除操作
    ASSERT_EQ(skipList.get(1), nullptr);
    ASSERT_FALSE(skipList.remove(1));

    // 插入一个键并多次更新它
    skipList.put(1, 100);
    ASSERT_EQ(*skipList.get(1), 100);
    skipList.put(1, 200);
    ASSERT_EQ(*skipList.get(1), 200);
    skipList.put(1, 300);
    ASSERT_EQ(*skipList.get(1), 300);

    // 删除键并尝试重新删除
    ASSERT_TRUE(skipList.remove(1));
    ASSERT_FALSE(skipList.remove(1));
    ASSERT_EQ(skipList.get(1), nullptr);
}

TEST(LockFreeSkipListTest, OverwriteAndRetrieve)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // 插入一个值，然后覆盖它
    skipList.put(5, 500);
    skipList.put(5, 600);
    int *val = skipList.get(5);

    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 600);
}

TEST(LockFreeSkipListTest, InsertDeleteInsertRetrieve)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // 插入、删除，然后再插入相同的键
    skipList.put(7, 700);
    skipList.remove(7);
    skipList.put(7, 800);
    int *val = skipList.get(7);

    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 800);
}

TEST(LockFreeSkipListTest, NonExistentElement)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // 查找不存在的元素
    int *val = skipList.get(999);
    EXPECT_EQ(val, nullptr);
}

TEST(LockFreeSkipListTest, RemoveNonExistentMultipleTimes)
{
    LockFreeSkipList<int, int, Comparator> skipList;

    // 多次尝试删除一个不存在的值
    bool removedFirstTime = skipList.remove(8);
    bool removedSecondTime = skipList.remove(8);

    EXPECT_FALSE(removedFirstTime);
    EXPECT_FALSE(removedSecondTime);
}

TEST(LockFreeSkipListTest, BasicOperations) {
    LockFreeSkipList<int, int, Comparator> skipList;

    skipList.put(5, 50);
    skipList.put(3, 30);
    skipList.put(8, 80);

    // Test get method
    ASSERT_EQ(*skipList.get(5), 50);
    ASSERT_EQ(*skipList.get(3), 30);
    ASSERT_EQ(skipList.get(7), nullptr);

    // Test update value
    skipList.put(5, 55);
    ASSERT_EQ(*skipList.get(5), 55);

    // Test remove method
    ASSERT_TRUE(skipList.remove(5));
    ASSERT_EQ(skipList.get(5), nullptr);
    ASSERT_FALSE(skipList.remove(5));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
