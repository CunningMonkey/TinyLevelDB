#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "iterator.h"
#include "slice.h"
#include "block_builder.h"

void BuildTable(Iterator *it, const char *filename);

extern size_t BLOCKSIZE;

class Index
{
private:
    Slice _key;
    uint32_t _offset;
    uint32_t _size;

public:
    Index(Slice key, uint32_t offset, uint32_t _size) : _key(key), _offset(offset), _size(_size) {}
    Slice Key() const { return _key; }
    inline uint32_t Offset() const { return _offset; }
    inline uint32_t Size() const { return _size; }
};

class TableBuilder
{
public:
    void Add(const Slice &key, const Slice &value);
    void Finish();
    void Flush();

    TableBuilder(const char *filename) : _table_file(filename, std::ios::app), _offset(0)
    {
    }

    ~TableBuilder()
    {
        _table_file.close();
    }

private:
    std::ofstream _table_file;
    BlockBuilder _block_builder;
    std::vector<Index> _indexes;
    uint32_t _offset;

    std::string encode_index(const Index &index)
    {
        std::string result;
        uint32_t offset = index.Offset();
        uint32_t size = index.Size();
        result.append(reinterpret_cast<const char *>(offset), sizeof(offset));
        result.append(reinterpret_cast<const char *>(size), sizeof(size));
        return result;
    }
};