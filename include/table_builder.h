#pragma once
#include <fstream>
#include "iterator.h"

void BuildTable(Iterator *it, const char *filename);

class TableBuilder
{
public:
    void Add(const char *key, const char *value);

    TableBuilder(const char *filename) : _table_file(filename, std::ios::app) {}

private:
    std::ofstream _table_file;
};