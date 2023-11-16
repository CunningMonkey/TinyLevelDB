#pragma once
#include <fstream>
#include "iterator.h"
#include "slice.h"

void BuildTable(Iterator *it, const char *filename);

class TableBuilder
{
public:
    void Add(const Slice &key, const Slice &value);
    void Finish();

    TableBuilder(const char *filename) : _table_file(filename, std::ios::app), _count(0)
    {
    }

    ~TableBuilder()
    {
        _table_file.close();
    }

private:
    std::ofstream _table_file;
};