#include "../src/skiplist.h" // Assuming your class is saved in this header file
#include <gtest/gtest.h>
#include <thread>
#include <algorithm>

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

int random_int(int min, int max)
{
    return min + std::rand() % (max - min + 1);
}

TEST(SkipListTest, PutAndGet)
{
    Arena arena;
    Comparator cmp;
    SkipList<int, Comparator> skipList(cmp, &arena);

    skipList.Put(101);
    int *val = skipList.Get(101);

    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 101);
}

TEST(SkipListTest, MultipleInsertAndRetrieve)
{
    Arena arena;
    Comparator cmp;
    SkipList<int, Comparator> skipList(cmp, &arena);
    const int N = 10;

    for (int i = 0; i < N; i++)
    {
        skipList.Put(i * 100);
    }

    std::vector<int *> keys = skipList.iterate();
    for (int i = 0; i < N; i++)
    {
        int key = *keys[i];
        ASSERT_EQ(key, i * 100);
    }
}

TEST(SkipListTest, RandomOperations)
{
    Comparator cmp;
    Arena arena;
    SkipList<int, Comparator> skipList(cmp, &arena);
    std::vector<int> reference;

    for (int i = 0; i < 100; ++i)
    {
        int key = random_int(1, 100);

        skipList.Put(key);
        reference.push_back(key);
    }
    std::sort(reference.begin(), reference.end());

    std::vector<int *>
        keys = skipList.iterate();
    for (int i = 0; i < 100; i++)
    {
        ASSERT_EQ(reference[i], *(keys[i]));
    }
}

class ConcurrentTest
{
private:
    static constexpr int K = 10000; // Number of keys
    SkipList<int, Comparator> _list;
    // std::atomic<int> _counter;
    Arena _arena;
    Comparator _cmp;

public:
    ConcurrentTest() : _list(_cmp, &_arena)
    {
    }

    void WriteStep()
    {
        for (int i = 0; i < K; ++i)
        {
            _list.Put(i);
        }
    }

    void ReadStep()
    {
        for (int i = 0; i < 10; i++)
        {
            auto val = _list.Get(i);
            if (val)
            {
                ASSERT_EQ(*val, i); // check if the value is double the key
            }
        }
    }

    void Run()
    {
        std::vector<std::thread> readers;

        for (int i = 0; i < 10; i++)
        {
            readers.push_back(std::thread(&ConcurrentTest::ReadStep, this));
        }
        std::thread writer(&ConcurrentTest::WriteStep, this);

        writer.join();
        for (auto &t : readers)
            t.join();
        for (int i = 0; i < K; i++)
        {
            auto val = _list.Get(i);
            ASSERT_NE(val, nullptr);
            ASSERT_EQ(*val, i);
        }
    }
};

TEST(SkipListTest, ConcurrentReadWrite)
{
    ConcurrentTest test;
    test.Run();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
