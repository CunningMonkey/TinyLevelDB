#include "memtable.h"
#include <gtest/gtest.h>
#include <random>

class MemTableTest : public ::testing::Test
{
protected:
    MemTable table;
};

TEST_F(MemTableTest, PutAndGet)
{
    Slice key("key1");
    Slice value("value1");
    table.Put(key, value, 1);

    std::string result;
    bool found = table.Get(key, 1, result);

    EXPECT_TRUE(found);
    EXPECT_EQ(result, "value1");
}

TEST_F(MemTableTest, Delete)
{
    Slice key("key2");
    Slice value("value2");
    table.Put(key, value, 1);

    std::string result;
    bool found = table.Get(key, 1, result);
    EXPECT_TRUE(found);
    EXPECT_EQ(result, "value2");

    table.Delete(key, 2);
    found = table.Get(key, 2, result);
    EXPECT_FALSE(found);
}

TEST_F(MemTableTest, BoundaryTest)
{
    Slice emptyKey("");
    Slice emptyValue("");
    Slice value("value");
    table.Put(emptyKey, value, 1);
    std::string result;
    bool found = table.Get(emptyKey, 1, result);
    EXPECT_TRUE(found);
    EXPECT_EQ(result, "value");
    table.Put(emptyKey, emptyValue, 2);
    found = table.Get(emptyKey, 2, result);
    EXPECT_TRUE(found);
    EXPECT_EQ(result, "");
}

TEST_F(MemTableTest, RepeatedKeyTest)
{
    Slice key("key");
    Slice value1("value1");
    Slice value2("value2");
    table.Put(key, value1, 1);
    table.Put(key, value2, 2);
    std::string result;
    bool found = table.Get(key, 2, result);
    EXPECT_TRUE(found);
    EXPECT_EQ(result, "value2");
}

TEST_F(MemTableTest, MultipleDeleteTest)
{
    Slice key("key");
    Slice value("value");
    table.Put(key, value, 1);
    table.Delete(key, 2);
    table.Delete(key, 3);
    std::string result;
    bool found = table.Get(key, 3, result);
    EXPECT_FALSE(found);
}

TEST_F(MemTableTest, MultipleVersionPut) {
    Slice key0("key0");
    Slice key1("key1");
    Slice value1("value1");
    Slice value2("value2");
    Slice value3("value3");
    table.Put(key0, value3, 1);
    table.Put(key1, value1, 2);
    table.Put(key1, value2, 3);
    table.Put(key1, value3, 4);
    std::string result;
    bool found = table.Get(key1, 3, result);
    EXPECT_EQ(result, value2.ToString());
}

TEST_F(MemTableTest, SequentialTest)
{
    for (uint64_t i = 0; i < 1000; ++i)
    {
        Slice key(std::to_string(i));
        Slice value("value" + std::to_string(i));
        table.Put(key, value, i);
    }
    for (uint64_t i = 0; i < 1000; ++i)
    {
        Slice key(std::to_string(i));
        std::string expected = "value" + std::to_string(i);
        std::string result;
        bool found = table.Get(key, i, result);
        EXPECT_TRUE(found);
        EXPECT_EQ(result, expected);
    }
}

TEST_F(MemTableTest, RandomOperationTest)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);

    for (uint64_t i = 0; i < 1000; ++i)
    {
        int operation = dis(gen);
        Slice key(std::to_string(i));
        switch (operation)
        {
        case 1: // Put
            table.Put(key, key, i);
            break;
        case 2: // Delete
            table.Delete(key, i);
            break;
        case 3: // Get
            std::string result;
            table.Get(key, i, result);
            break;
        }
    }
}

class IteratorTest : public ::testing::Test
{
protected:
    MemTable table;

    void SetUp() override
    {
        Slice key1("key1");
        Slice key2("key2");
        Slice key3("key3");
        Slice value1("value1");
        Slice value2("value2");
        Slice value3("value3");
        table.Put(key1, value1, 1);
        table.Put(key2, value2, 2);
        table.Put(key3, value3, 3);
    }
};

TEST_F(IteratorTest, IterateThroughElements)
{
    std::unique_ptr<Iterator> iter = std::unique_ptr<Iterator>(table.NewIterator());

    iter->SeekToFirst();
    ASSERT_TRUE(iter->Valid());
    std::string key(iter->Key().data(),  iter->Key().size()-8);
    EXPECT_EQ(key, "key1");
    EXPECT_EQ(iter->Value().ToString(), "value1");

    iter->Next();
    ASSERT_TRUE(iter->Valid());
    std::string key2(iter->Key().data(),  iter->Key().size()-8);
    EXPECT_EQ(key2, "key2");
    EXPECT_EQ(iter->Value().ToString(), "value2");

    iter->Next();
    ASSERT_TRUE(iter->Valid());
    std::string key3(iter->Key().data(),  iter->Key().size()-8);
    EXPECT_EQ(key3, "key3");
    EXPECT_EQ(iter->Value().ToString(), "value3");

    iter->Next();
    ASSERT_FALSE(iter->Valid());
}

TEST_F(IteratorTest, IterateEmptyTable)
{
    MemTable emptyTable;
    std::unique_ptr<Iterator> iter = std::unique_ptr<Iterator>(emptyTable.NewIterator());

    iter->SeekToFirst();
    ASSERT_FALSE(iter->Valid());
}

TEST_F(IteratorTest, EndOfTable)
{
    std::unique_ptr<Iterator> iter = std::unique_ptr<Iterator>(table.NewIterator());

    iter->SeekToFirst();
    while (iter->Valid())
    {
        iter->Next();
    }

    ASSERT_FALSE(iter->Valid());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
