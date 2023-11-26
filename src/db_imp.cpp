// db_imp.cpp
#include "db_imp.h"

DBImpl::DBImpl() {}

DBImpl::~DBImpl() {}

bool DBImpl::Open(const std::string name)
{
    _memtable = new MemTable;
    _db = name;
    return true;
}

bool DBImpl::Put(const Slice &key, const Slice &value)
{
    _memtable->Put(key, value, _sequence_num++);
    if (_memtable->ApproximateMemoryUsage() > _options.write_buffer_size)
    {
        while (_immutable_memtable != nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        _immutable_memtable = _memtable;
    }
}

bool DBImpl::Get(const Slice &key, std::string &value)
{
    bool found = _memtable->Get(key, _sequence_num, value);
    if (found)
        return true;
    return false;
}

bool DBImpl::Delete(const Slice &key)
{
    _memtable->Delete(key, _sequence_num++);
    return false;
}

void BackgroundCompactMemTable()
{
    // Dump immutable memtable to disk
}