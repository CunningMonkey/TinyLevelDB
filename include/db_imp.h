#pragma once
#include "memtable.h"
#include "options.h"
#include "slice.h"
#include "table_builder.h"
#include "version.h"
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

class DBImpl {
  public:
    DBImpl();
    ~DBImpl();

    bool Open(const std::string name);
    bool Put(const Slice &key, const Slice &value);
    bool Get(const Slice &key, std::string &value);
    bool Delete(const Slice &key);
    bool Start();
    void CompactMemTable();
    void BackgroundCompaction();

  private:
    std::string _db;
    MemTable *_memtable;
    MemTable *_immutable_memtable;
    uint64_t _sequence_num;
    const Options _options;
    Version _version;
    uint64_t _current;
    std::mutex _mtx;
    std::condition_variable _cond_var;
};
