#include "slice.h"

class Iterator
{
public:
    Iterator();

    Iterator(const Iterator &) = delete;
    Iterator &operator=(const Iterator &) = delete;
    virtual ~Iterator();
    virtual void SeekToFirst() = 0;
    virtual void Next() = 0;
    virtual bool Valid() const = 0;
    virtual Slice Key() const = 0;
    virtual Slice Value() const = 0;
};