#pragma once
#include <string>
#include "memtable.h"

class SSTable
{
private:
    MemTable *_mem_table;
    const std::string *_filename;

public:
    SSTable(MemTable *const mem_table, const std::string *const filename) : _mem_table(mem_table), _filename(filename)
    {
        _mem_table->Ref();
    }

    ~SSTable()
    {
        _mem_table->UnRef();
    }

    void Dump()
    {
    }
};