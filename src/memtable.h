#pragma once

#include <string>
#include <string>
#include "skiplist.h"
#include "comparator.h"
#include "iterator.h"
#include "util/encoding.h"

class MemTableIterator;

class MemTable
{
private:
    typedef SkipList<const char *, InternalKeyComparator> Table;
    Table _list;
    InternalKeyComparator _cmp;
    Arena _arena;
    int _reference;
    KeyComparator _key_cmp;

    friend class MemTableIterator;

    // ~MemTable();

public:
    MemTable() : _list(_cmp, &_arena), _reference(0) {}
    ~MemTable();

    void Put(Slice &key, Slice &value, uint64_t sequence_num);
    void Delete(Slice &key, uint64_t sequence_num);
    bool Get(Slice &key, uint64_t sequence_num, std::string &value);

    void Ref() { ++_reference; }
    void UnRef() { --_reference; }

    Iterator *NewIterator();
};
