#pragma once
#include <string>
#include <vector>
#include "slice.h"
#include <fstream>

extern size_t RESTART_INTERVAL;

class BlockBuilder
{
public:
    BlockBuilder() : _counter(0), _record_restart(false), _buffer(), _restarts() {}

    void Reset();
    void Add(const Slice &key, const Slice &value);
    Slice Finish();
    size_t Size() const { return _buffer.size(); }
    const Slice &FirstKey() { return _first_key; }

private:
    std::string _buffer;
    std::vector<uint32_t> _restarts;
    size_t _counter;
    bool _record_restart;
    Slice _first_key;
};

void WriteBlock(BlockBuilder &_block_builder, std::ofstream &out);