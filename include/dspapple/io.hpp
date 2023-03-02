#pragma once

#include <cstdio>

namespace dspapple
{
    // Wrappers around fwrite and fread since their API is kinda shit
    size_t read_from_file(FILE* file, void* ptr, size_t bytes);
    size_t write_to_file(FILE* file, void* ptr, size_t bytes);
}
