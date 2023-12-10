#include "memtable.h"
#include "encoding.h"
#include "slice.h"

void MemTable::Put(const Slice &key, const Slice &value,
                   const uint64_t sequence_num) {
    size_t seq_size = sizeof(sequence_num);
    size_t key_length = key.size() + seq_size;
    size_t size_length = sizeof(size_t);
    size_t value_length = value.size();
    uint8_t tag = (uint8_t)kTypeValue;
    char *new_key = _arena.Allocate(size_length + key_length + 1 + size_length +
                                    value_length);
    char *new_key_cp = new_key;
    std::memcpy(new_key, &key_length, size_length);
    new_key += size_length;
    std::memcpy(new_key, key.data(), key_length - seq_size);
    new_key += key_length - seq_size;
    std::memcpy(new_key, &sequence_num, seq_size);
    new_key += seq_size;
    std::memcpy(new_key, &tag, 1);
    new_key += 1;
    std::memcpy(new_key, &value_length, size_length);
    new_key += size_length;
    std::memcpy(new_key, value.data(), value_length);
    _list.Put(new_key_cp);
}

void MemTable::Delete(const Slice &key, const uint64_t sequence_num) {
    size_t seq_size = sizeof(sequence_num);
    size_t key_length = key.size() + seq_size;
    size_t size_length = sizeof(size_t);
    uint8_t tag = (uint8_t)kTypeDeletion;
    char *new_key = _arena.Allocate(size_length + key_length + 1);
    char *new_key_cp = new_key;
    std::memcpy(new_key, &key_length, size_length);
    new_key += size_length;
    std::memcpy(new_key, key.data(), key_length - seq_size);
    new_key += key_length - seq_size;
    std::memcpy(new_key, &sequence_num, seq_size);
    new_key += seq_size;
    std::memcpy(new_key, &tag, 1);
    new_key += 1;
    _list.Put(new_key_cp);
}

bool MemTable::Get(const Slice &key, const uint64_t sequence_num,
                   std::string &value) {
    size_t seq_size = sizeof(sequence_num);
    size_t key_length = key.size() + seq_size;
    size_t size_length = sizeof(size_t);
    std::string lookup_key(sizeof(size_t) + key_length, '\0');
    std::memcpy(&lookup_key[0], &key_length, size_length);
    std::memcpy(&lookup_key[size_length], key.data(), key_length - seq_size);
    std::memcpy(&lookup_key[key_length + size_length - seq_size], &sequence_num,
                seq_size);
    const char **const res = _list.Get(lookup_key.c_str());
    if (res == nullptr) {
        return false;
    }
    if (_key_cmp(*res, lookup_key.c_str()) == 0) {
        uint8_t tag = *(
            reinterpret_cast<const uint8_t *>(*res + size_length + key_length));
        if (!(tag & kTypeValue)) {
            return false;
        }
        int value_length = *(reinterpret_cast<const size_t *>(
            *res + size_length + key_length + 1));
        value.assign(*res + size_length + key_length + 1 + size_length,
                     value_length);
        return true;
    } else {
        return false;
    }
}

Iterator *MemTable::NewIterator() { return new MemTableIterator(&_list); }
