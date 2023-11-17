#include "table_builder.h"

void BuildTable(Iterator *iter, const char *filename)
{
    iter->SeekToFirst();
    TableBuilder *builder = new TableBuilder(filename);

    while (iter->Valid())
    {
        builder->Add(iter->Key(), iter->Value());
        iter->Next();
    }
}

void TableBuilder::Add(const Slice &key, const Slice &value)
{
    _block_builder.Add(key, value);
    if (_block_builder.Size() >= BLOCKSIZE)
    {
        _table_file << _block_builder.Finish();
        _block_builder.Reset();
    }
}

void TableBuilder::Finish()
{
}