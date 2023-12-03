#include "table_builder.h"

size_t BLOCKSIZE = 4096;

void TableBuilder::Add(const Slice &key, const Slice &value) {
    _block_builder.Add(key, value);
    if (_block_builder.Size() >= BLOCKSIZE) {
        Flush();
    }
}

void TableBuilder::Finish() {
    Flush();
    BlockBuilder index_builder;
    for (size_t i = 0; i < _indexes.size(); i++) {
        Slice key = _indexes[i].Key();
        uint32_t offset = _indexes[i].Offset();
        uint32_t size = _indexes[i].Size();
        Slice value = Slice(encode_index(_indexes[i]));
        index_builder.Add(key, value);
    }
    WriteBlock(index_builder, _table_file);
    uint32_t index_size = _indexes.size();
    _table_file.write(reinterpret_cast<const char *>(&index_size),
                      sizeof(index_size));
}

void TableBuilder::Flush() {
    WriteBlock(_block_builder, _table_file);

    _indexes.push_back(
        Index(_block_builder.FirstKey(), _offset, _block_builder.Size()));
    _offset += _block_builder.Size();

    _block_builder.Reset();
}
