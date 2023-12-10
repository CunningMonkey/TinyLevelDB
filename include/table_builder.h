#pragma once
#include "block_builder.h"
#include "iterator.h"
#include "slice.h"
#include <fstream>
#include <string>
#include <vector>

extern size_t BLOCKSIZE;

class Index {
  private:
    Slice _key;
    uint32_t _offset;
    uint32_t _size;

  public:
    Index(Slice key, uint32_t offset, uint32_t _size)
        : _key(key), _offset(offset), _size(_size) {}
    Slice Key() const { return _key; }
    inline uint32_t Offset() const { return _offset; }
    inline uint32_t Size() const { return _size; }
};

class TableBuilder {
  public:
    void Add(const Slice &key, const Slice &value);
    void Finish();
    std::string encode_index(const Index &index);

    TableBuilder(std::string &fileName)
        : _table_file(fileName, std::ios::app), _offset(0) {}

    ~TableBuilder() { _table_file.close(); }

  private:
    void flush();
    void appendFooter();
    std::ofstream _table_file;
    BlockBuilder _block_builder;
    std::vector<Index> _indexes;
    uint32_t _offset;

};
