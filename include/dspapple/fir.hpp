#pragma once

#include <cstdint>
#include <complex>
#include "dspapple/utils.hpp"

namespace dspapple
{
    enum class data_type_t { float32, complex32 };

    struct fir_buffer
    {
        void* m_pBuffer = nullptr;
        bool m_bAllocated = false;
        data_type_t m_eDatatype = data_type_t::float32;
        std::uint32_t m_uSamples = 0;
        std::uint32_t m_uBufferSize = 0; // how many samples total in the buffer

        void init(std::uint32_t samples, std::uint32_t window_size, data_type_t data_type);
        void init_ptr(float* input, std::uint32_t array_size, std::uint32_t window_size);
        void init_ptr(std::complex<float>* input, std::uint32_t array_size, std::uint32_t window_size);
        void deallocate();
        void* get_input_dest();
        ~fir_buffer();
        std::size_t get_input_bytes();
        std::size_t get_window_size();
    };

    enum class window_type { none, hamming };

    struct fir_filter
    {
        float* array = nullptr; // The filter weights are inverted for performance
        uint32_t tap_count = 0;

        ~fir_filter();
        void init(std::uint32_t taps, window_type window);
        error_code decimate(fir_buffer* input, fir_buffer* output, std::uint32_t decimation);
    };
}
