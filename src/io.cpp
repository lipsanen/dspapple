#include "dspapple/io.hpp"
#include <cstdint>
#include <cstring>

size_t dspapple::read_from_file(FILE* file, void* _ptr, size_t bytes)
{
    std::uint8_t* ptr = (std::uint8_t*)_ptr;

    size_t bytesLeft = bytes;
    while(bytesLeft > 0)
    {
        auto bytesRead = fread(ptr, 1, bytesLeft, stdin);
        if(bytesRead <= 0)
        {
            break;
        }
        ptr += bytesRead;
        bytesLeft -= bytesRead;
    }

    // 0 out the rest
    if(bytesLeft > 0)
    {     
        memset(ptr + (bytes - bytesLeft), 0, bytesLeft);
    }

    return bytes - bytesLeft;
}

size_t dspapple::write_to_file(FILE* file, void* _ptr, size_t bytes)
{
    std::uint8_t* ptr = (std::uint8_t*)_ptr;

    size_t bytesLeft = bytes;
    while(bytesLeft > 0)
    {
        auto bytesWritten = fwrite(ptr, 1, bytesLeft, stdout);
        if(bytesWritten <= 0)
        {
            break;
        }
        ptr += bytesWritten;
        bytesLeft -= bytesWritten;
    }

    return bytes - bytesLeft;
}