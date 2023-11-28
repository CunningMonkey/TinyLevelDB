#pragma once
#include <cstddef>

struct Options
{
    size_t block_size = 4096;
    size_t write_buffer_size = 4096 * 1024;
    size_t max_level = 7;
};
