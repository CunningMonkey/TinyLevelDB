#ifndef MEM_TABLE_H
#define MEM_TABLE_H

#include <string>
#include <string>
#include "skiplist.h"
#include "comparator.h"

class MemTable {
private:
    SkipList<const char*, InternalKeyComparator> _list;
    InternalKeyComparator _cmp;
    Arena _arena;
    int _reference;

public:
    MemTable(): _list(_cmp, &_arena), _reference(0) {}

    void Put(Slice& key, Slice& value, uint64_t sequence_num);
    void Delete(Slice& key, uint64_t sequence_num);
    bool Get(Slice& key, uint64_t sequence_num, std::string& value);
};

#endif