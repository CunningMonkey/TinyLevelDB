#include "memtable.h"

void MemTable::Put(Slice &key, Slice &value, uint64_t sequence_num)
{
    size_t key_length = key.size();
    size_t size_length = sizeof(size_t);
    size_t value_length = value.size();
    char *new_key = _arena.Allocate(key_length + size_length * 2 + value_length + 8);
    uint64_t tag = (uint64_t)((sequence_num << 1) | kTypeValue);
    std::memcpy(new_key, &key_length, size_length);
    std::memcpy(new_key+size_length, key.data(), key_length);
    std::memcpy(new_key+size_length+key_length, &tag, 8);
    std::memcpy(new_key+size_length+key_length+8, &value_length, size_length);
    std::memcpy(new_key+size_length+key_length*2+8, value.data(), value_length);
    _list.Put(new_key);
}

void MemTable::Delete(Slice &key, uint64_t sequence_num)
{
    size_t key_length = key.size();
    size_t size_length = sizeof(size_t);
    char *new_key = _arena.Allocate(key_length + size_length + 8);
    uint64_t tag = (uint64_t)((sequence_num << 1) | kTypeDeletion);
    std::memcpy(new_key, &key_length, size_length);
    std::memcpy(new_key+size_length, key.data(), key_length);
    std::memcpy(new_key+size_length+key_length, &tag, 8);
    _list.Put(new_key);
}

bool MemTable::Get(Slice &key, uint64_t sequence_num, std::string &value)
{
    int key_length = key.size();
    int size_length = sizeof(size_t);
    uint64_t s_num = (sequence_num << 1);
    std::string lookup_key(sizeof(size_t)+key.size()+8, '\0');
    std::memcpy(&lookup_key[0], &key_length, size_length);
    std::memcpy(&lookup_key[size_length], key.data(), key_length);
    std::memcpy(&lookup_key[key_length+size_length], &s_num, 8);
    const char* const res = *(_list.Get(lookup_key.c_str()));
    if (res == nullptr) {
        return false;
    }
    if (_cmp(res, lookup_key.c_str()) == 0) {
        // uint64_t 
    } else {
        return false;
    }
}
