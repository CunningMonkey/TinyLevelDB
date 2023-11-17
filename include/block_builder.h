#pragma once
#include <string>
#include <vector>
#include "slice.h"
#include <fstream>

void WriteBlock(const std::string &buffer, const std::vector<uint32_t> &restarts, std::ofstream &out)
{
    out.write(buffer.data(), buffer.size());

    // Write the restarts to the output stream
    for (const auto restart : restarts)
    {
        out.write(reinterpret_cast<const char *>(&restart), sizeof(restart));
    }

    // writer the number of restarts to the output stream file at the BLOCKSIZE - sizeof(uint32_t) offset
    uint32_t num_restarts = restarts.size();
}

class BlockBuilder
{
public:
    BlockBuilder();
    ~BlockBuilder();

    void Reset();
    bool Add(const Slice &key, const Slice &value);
    std::string Finish();
    size_t Size() const { return buffer_.size(); }

private:
    std::string buffer_;
    std::vector<uint32_t> restarts_;
    int counter_;
    bool finished_;
};
