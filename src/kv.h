#include "SkipList.h"

struct Comparator
{
    int operator()(const char* a, const char* b) const
    {
        return strcmp(a, b);
    }
};

Comparator _cmp;

class KV {
public:
    KV(): _list(_cmp) {}

    void put(char* key, char* value) {

    }

private:
    SkipList<char*, char*, Comparator> _list;
};