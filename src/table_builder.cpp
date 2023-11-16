#include "table_builder.h"

void BuildTable(Iterator *iter, const char *f)
{
    iter->SeekToFirst();
    while (iter->Valid())
    {

        iter->Next();
    }
}