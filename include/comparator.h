#ifndef COMPARATOR
#define COMPARATOR

#include "slice.h"

static Slice GetLengthPrefixedSlice(const char *data)
{
    size_t len;
    const char *p = data;
    // p = GetVarint32Ptr(p, p + 5, &len);
    len = *(reinterpret_cast<const size_t *>(p));
    p += sizeof(size_t);
    return {p, len};
}

struct InternalKeyComparator
{
    int operator()(const char *const key1, const char *const key2)
    {
        size_t size_t_size = sizeof(size_t);
        size_t length1 = *(reinterpret_cast<const size_t *>(key1)) - 8;
        size_t length2 = *(reinterpret_cast<const size_t *>(key2)) - 8;
        const Slice k1 = Slice(key1 + size_t_size, length1);
        const Slice k2 = Slice(key2 + size_t_size, length2);
        int r = k1.compare(k2);
        if (r != 0)
        {
            return r;
        }
        assert(length1 == length2);
        uint64_t seq1 = *(reinterpret_cast<const uint64_t *>(key1 + size_t_size + length1)) >> 8;
        uint64_t seq2 = *(reinterpret_cast<const uint64_t *>(key2 + size_t_size + length2)) >> 8;
        if (seq1 == seq2)
        {
            return 0;
        }
        else if (seq1 < seq2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
};

struct KeyComparator
{
    int operator()(const char *const key1, const char *const key2)
    {
        size_t length1 = *(reinterpret_cast<const size_t *>(key1)) - 8;
        size_t length2 = *(reinterpret_cast<const size_t *>(key2)) - 8;
        const Slice k1 = Slice(key1 + sizeof(size_t), length1);
        const Slice k2 = Slice(key2 + sizeof(size_t), length2);
        return k1.compare(k2);
    }
};

#endif
