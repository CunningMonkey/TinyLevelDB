#pragma once

#include <string>

class BlockReader {
  public:
    BlockReader(const std::string buffer) : _buffer(buffer) {}
    const char *Get(const char *key);

  private:
    std::string _buffer;
};
