#ifndef COMPARATOR
#define COMPARATOR

#include "slice.h"

static Slice GetLengthPrefixedSlice(const char* data) {
    size_t len;
    const char* p = data;
    // p = GetVarint32Ptr(p, p + 5, &len);  
    len = *(reinterpret_cast<const size_t*>(p));
    p += sizeof(size_t);
    return {p, len};
}

struct InternalKeyComparator {
    int operator() (const char* const key1, const char* const key2) {
        const Slice k1 = GetLengthPrefixedSlice(key1);
        const Slice k2 = GetLengthPrefixedSlice(key2);
        return k1.compare(k2);
    }
};

#endif