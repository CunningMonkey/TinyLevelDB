#include <gtest/gtest.h>
#include "table_builder.h"

class TableBuilderTest : public ::testing::Test
{
protected:
    TableBuilder *tableBuilder;

    void SetUp() override
    {
        // 设置一个临时文件名用于测试
        tableBuilder = new TableBuilder("temp_test_file");
    }

    void TearDown() override
    {
        delete tableBuilder;
        // 删除或清理测试用的文件
    }
};

class MockIterator : public Iterator
{
public:
    MockIterator() : _keys(), _values(), _index(0) {}

    void Add(const std::string &key, const std::string &value)
    {
        _keys.push_back(key);
        _values.push_back(value);
    }

    bool Valid() const override
    {
        return _index < _keys.size();
    }

    void SeekToFirst() override
    {
        _index = 0;
    }

    void Next() override
    {
        _index++;
    }

    Slice Key() const override
    {
        return Slice(_keys[_index]);
    }

    Slice Value() const override
    {
        return Slice(_values[_index]);
    }

private:
    std::vector<std::string> _keys;
    std::vector<std::string> _values;
    size_t _index;
};

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
