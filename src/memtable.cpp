#include "memtable.h"

inline const Slice GetKeyFromInternalKey(const char *const internalKey)
{
    size_t key_length = *(reinterpret_cast<const size_t *>(internalKey)) - 8;
    const Slice key = Slice(internalKey + sizeof(size_t), key_length);
    return key;
}

inline const Slice GetValueFromInternalKey(const char *const internalKey)
{
    const Slice key = GetKeyFromInternalKey(internalKey);
    const char *const value_start = internalKey + key.size() + sizeof(size_t) + 8;
    size_t value_length = *(reinterpret_cast<const size_t *>(value_start));
    const Slice value = Slice(value_start + sizeof(size_t), value_length);
    return value;
}

class MemTableIterator : public Iterator
{
private:
    MemTable::Table::Iterator _iter;

public:
    explicit MemTableIterator(MemTable::Table *table) : _iter(table) {}

    MemTableIterator(const MemTableIterator &) = delete;
    MemTableIterator &operator=(const MemTableIterator &) = delete;

    ~MemTableIterator() override = default;
    bool Valid() const override
    {
        return _iter.Valid();
    }

    void Next() override
    {
        _iter.Next();
    }

    void SeekToFirst() override
    {
        _iter.SeekToFirst();
    }

    Slice Key() const override
    {
        return GetKeyFromInternalKey(_iter.key());
    }

    Slice Value() const override
    {
        return GetValueFromInternalKey(_iter.key());
    }
};

MemTable::~MemTable()
{
    assert(_reference == 0);
}

void MemTable::Put(Slice &key, Slice &value, uint64_t sequence_num)
{
    size_t key_length = key.size() + 8;
    size_t size_length = sizeof(size_t);
    size_t value_length = value.size();
    char *new_key = _arena.Allocate(key_length + size_length * 2 + value_length);
    uint64_t tag = (uint64_t)((sequence_num << 8) | kTypeValue);
    std::memcpy(new_key, &key_length, size_length);
    std::memcpy(new_key + size_length, key.data(), key_length);
    std::memcpy(new_key + size_length + key_length - 8, &tag, 8);
    std::memcpy(new_key + size_length + key_length, &value_length, size_length);
    std::memcpy(new_key + size_length * 2 + key_length, value.data(), value_length);
    _list.Put(new_key);
}

void MemTable::Delete(Slice &key, uint64_t sequence_num)
{
    size_t key_length = key.size() + 8;
    size_t size_length = sizeof(size_t);
    char *new_key = _arena.Allocate(key_length + size_length);
    uint64_t tag = (uint64_t)((sequence_num << 8) | kTypeDeletion);
    std::memcpy(new_key, &key_length, size_length);
    std::memcpy(new_key + size_length, key.data(), key_length);
    std::memcpy(new_key + size_length + key_length - 8, &tag, 8);
    _list.Put(new_key);
}

bool MemTable::Get(Slice &key, uint64_t sequence_num, std::string &value)
{
    size_t key_length = key.size() + 8;
    size_t size_length = sizeof(size_t);
    uint64_t s_num = (sequence_num << 8);
    std::string lookup_key(sizeof(size_t) + key_length, '\0');
    std::memcpy(&lookup_key[0], &key_length, size_length);
    std::memcpy(&lookup_key[size_length], key.data(), key_length);
    std::memcpy(&lookup_key[key_length + size_length - 8], &s_num, 8);
    const char **const res = _list.Get(lookup_key.c_str());
    if (res == nullptr)
    {
        return false;
    }
    if (_key_cmp(*res, lookup_key.c_str()) == 0)
    {
        uint64_t tag = *(reinterpret_cast<const uint64_t *>(*res + key_length + size_length - 8));
        if (!(tag & kTypeValue))
        {
            return false;
        }
        int value_length = *(reinterpret_cast<const size_t *>(*res + key_length + size_length));
        value.assign(*res + key_length + size_length * 2, value_length);
        return true;
    }
    else
    {
        return false;
    }
}

Iterator *MemTable::NewIterator()
{
    return new MemTableIterator(&_list);
}
