#include "block_builder.h"

size_t RESTART_INTERVAL = 16;

void WriteBlock(BlockBuilder &block_builder, std::ofstream &out)
{
    Slice buffer = block_builder.Finish();
    out.write(buffer.data(), buffer.size());
}

void BlockBuilder::Reset()
{
    _buffer.clear();
    _restarts.clear();
    _counter = 0;
    _record_restart = false;
}

void BlockBuilder::Add(const Slice &key, const Slice &value)
{
    if (_buffer.size() == 0)
        _first_key = key;
    uint32_t key_length = key.size();
    uint32_t value_length = value.size();
    _buffer.append(reinterpret_cast<const char *>(&key_length), sizeof(key_length));
    _buffer.append(key.data(), key_length);
    _buffer.append(reinterpret_cast<const char *>(&value_length), sizeof(value_length));
    _buffer.append(value.data(), value_length);
    _counter++;
    _record_restart = false;
    if (_counter % RESTART_INTERVAL == 0)
    {
        _restarts.push_back(_buffer.size());
        _record_restart = true;
    }
}

Slice BlockBuilder::Finish()
{
    if (_buffer.size() == 0)
        return Slice();
    if (!_record_restart)
        _restarts.push_back(_buffer.size());
    for (size_t i = 0; i < _restarts.size(); i++)
    {
        uint32_t restart_offset = _restarts[i];
        _buffer.append(reinterpret_cast<const char *>(&restart_offset), sizeof(restart_offset));
    }
    uint32_t restarts_size = _restarts.size();
    _buffer.append(reinterpret_cast<const char *>(&restarts_size), sizeof(restarts_size));
    return Slice(_buffer);
}
