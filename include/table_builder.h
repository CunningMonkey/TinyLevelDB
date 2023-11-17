#pragma once
#include <fstream>
#include "iterator.h"
#include "slice.h"
#include "block_builder.h"

void BuildTable(Iterator *it, const char *filename);

size_t BLOCKSIZE = 4096;

class TableBuilder
{
public:
    void Add(const Slice &key, const Slice &value);
    void Finish();

    TableBuilder(const char *filename) : _table_file(filename, std::ios::app)
    {
    }

    ~TableBuilder()
    {
        _table_file.close();
    }

private:
    std::ofstream _table_file;
    BlockBuilder _block_builder;
};