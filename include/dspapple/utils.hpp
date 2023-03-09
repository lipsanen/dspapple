#pragma once

#include <string>
#include <cstddef>
#include <complex>

namespace dspapple
{
    struct error
    {
        std::string m_sErrorMessage;
        bool m_bError = false;
    };

    template<typename T, typename U>
    static inline void CastSamples(T* input, U* output, std::size_t count)
    {
        for(size_t i=0; i < count; ++i)
        {
            output[i] = input[i];
        }
    }
}
