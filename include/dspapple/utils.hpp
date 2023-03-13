#pragma once

#include <string>
#include <cstddef>
#include <complex>

namespace dspapple
{
    enum class error_code { success = 0, fir_input_output_mismatch, fir_input_window_size_doesnt_match_filter };

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
