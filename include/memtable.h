#pragma once

#include "comparator.h"
#include "encoding.h"
#include "iterator.h"
#include "skiplist.h"
#include <string>

class MemTableIterator;

inline const Slice
GetKeyAndSeqNumFromInternalKey(const char *const internalKey) {
    size_t key_length = *(reinterpret_cast<const size_t *>(internalKey));
    const Slice key = Slice(internalKey + sizeof(size_t), key_length);
    return key;
}

inline const Slice GetValueFromInternalKey(const char *const internalKey) {
    const Slice key = GetKeyAndSeqNumFromInternalKey(internalKey);
    const char *const value_start = key.data() + key.size();
    size_t value_length = *(reinterpret_cast<const size_t *>(value_start));
    const Slice value = Slice(value_start + sizeof(size_t), value_length);
    return value;
}

class MemTable {
  private:
    typedef SkipList<const char *, InternalKeyComparator> Table;
    Table _list;
    InternalKeyComparator _cmp;
    Arena _arena;
    int _reference;
    KeyComparator _key_cmp;

    friend class MemTableIterator;

  public:
    MemTable() : _list(_cmp, &_arena), _reference(0) {}

    void Put(const Slice &key, const Slice &value, const uint64_t sequence_num);
    void Delete(const Slice &key, const uint64_t sequence_num);
    bool Get(const Slice &key, const uint64_t sequence_num, std::string &value);

    // void Ref() { ++_reference; }
    // void UnRef()
    // {
    //     --_reference;
    //     if (_reference == 0)
    //     {
    //         delete this;
    //     }
    // }

    uint64_t ApproximateMemoryUsage() { return _arena.MemoryUsage(); }

    Iterator *NewIterator();
};

class MemTableIterator : public Iterator {
  private:
    MemTable::Table::Iterator _iter;

  public:
    explicit MemTableIterator(MemTable::Table *table) : _iter(table) {}

    MemTableIterator(const MemTableIterator &) = delete;
    MemTableIterator &operator=(const MemTableIterator &) = delete;

    ~MemTableIterator() override = default;
    bool Valid() const override { return _iter.Valid(); }

    void Next() override { _iter.Next(); }

    void SeekToFirst() override { _iter.SeekToFirst(); }

    Slice Key() const override {
        return GetKeyAndSeqNumFromInternalKey(_iter.key());
    }

    Slice Value() const override {
        return GetValueFromInternalKey(_iter.key());
    }
};
