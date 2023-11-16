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
    // Write key size and key
    uint32_t key_size = key.size();
    _table_file.write((const char *)&key_size, sizeof(key_size));
    _table_file.write(key.data(), key_size);

    // Write value size and value
    uint32_t value_size = value.size();
    _table_file.write((const char *)&value_size, sizeof(value_size));
    _table_file.write(value.data(), value_size);
}

void TableBuilder::Finish()
{
}