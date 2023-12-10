#pragma once
#include "slice.h"
#include <fstream>
#include <iostream>
#include <vector>

class TableReader {
  public:
    TableReader(const std::string file_name)
        : _file_stream(file_name, std::ios::binary | std::ios::ate) {
        if (!_file_stream.is_open()) {
            _ok = false;
            std::cerr << "Cannot open file for table builder " << file_name
                      << std::endl;
            return;
        }
        std::streampos file_size = _file_stream.tellg();
        if (file_size < 4) {
			_ok = false;
            std::cerr << "Table file size is less than 4" << std::endl;
			return;
        }
		_file_stream.seekg(-4, std::ios::end);
		std::vector<char> index_block_size(4);
		_file_stream.read(index_block_size.data(), 4);
    }
    const char *Get(const char *key);
    bool Ok() { return _ok; }

  private:
    bool _ok = true;
    void readFooter();
    std::ifstream _file_stream;
};
