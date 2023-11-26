// db_imp.h
#include <string>
#include <chrono>
#include <thread>
#include "memtable.h"
#include "slice.h"
#include "options.h"

class DBImpl
{
public:
    DBImpl();
    ~DBImpl();

    bool Open(const std::string name);
    bool Put(const Slice &key, const Slice &value);
    bool Get(const Slice &key, std::string &value);
    bool Delete(const Slice &key);
    void CompactMemTable();

private:
    std::string _db;
    MemTable *_memtable;
    MemTable *_immutable_memtable;
    uint64_t _sequence_num;
    const Options _options;
};