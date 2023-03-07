#pragma once

#include <cstdint>
#include <complex>

namespace dspapple
{
    enum class data_type_t { float32, complex32 };

    struct fir_state
    {
        std::size_t input_offset = 0;
        std::uint32_t input_samples = 0;
        std::uint32_t output_samples = 0;
        data_type_t data_type = data_type_t::float32;
        void* input = nullptr;
        void* output = nullptr;

        // can init with complex or float data
        void init(std::complex<float>* input, std::uint32_t input_samples, std::complex<float>* output, std::uint32_t output_samples);
        void init(float* input, std::uint32_t input_samples, float* output, std::uint32_t output_samples);
        void* get_input_dest(); // get pointer where we want to memcpy new inputs
        std::uint32_t bytes_wanted(); // how many bytes do we want on this iteration
    };

    struct fir_filter
    {
        float* array = nullptr;
        uint32_t tap_count = 0;

        ~fir_filter();
        void init(std::uint32_t taps);
        void decimate(fir_state* state, std::uint32_t decimation);
    };
}
