#include "dspapple/fir.hpp"
#include <string.h>

#ifdef __AVX2__
#define SSE
#define AVX
#include "simd_utils/simd_utils.h"
#endif

using namespace dspapple;

struct float_vec {
    union {
        alignas(64) v8sf vec;
        alignas(64) float values[8];
    };
};

fir_filter::~fir_filter()
{
    if(this->array)
    {
        free(this->array);
    }
}

void fir_state::init(std::complex<float>* input, std::uint32_t input_samples, std::complex<float>* output, std::uint32_t output_samples)
{
    this->data_type = data_type_t::complex32;
    this->input = input;
    this->input_samples = input_samples;
    this->output = output;
    this->output_samples = output_samples;
}

void fir_state::init(float* input, std::uint32_t input_samples, float* output, std::uint32_t output_samples)
{
    this->data_type = data_type_t::float32;
    this->input = input;
    this->input_samples = input_samples;
    this->output = output;
    this->output_samples = output_samples;
}

void* fir_state::get_input_dest()
{
    if(data_type == data_type_t::float32)
        return ((float*)input) + input_offset;
    else
        return ((std::complex<float>*)input) + input_offset;
}

std::uint32_t fir_state::bytes_wanted()
{
    if(data_type == data_type_t::float32)
        return (input_samples - input_offset) * 4;
    else
        return (input_samples - input_offset) * 8;
}

static size_t find_aligned_size(size_t size, size_t alignment)
{
    if((size % alignment) != 0)
    {
        return (size / alignment + 1) * alignment;
    }
    else
    {
        return size;
    }
}

void fir_filter::init(std::uint32_t taps)
{
    this->tap_count = taps;
    this->array = (float*)aligned_alloc(64, find_aligned_size(taps * sizeof(float), 64));
}

static void decimate_complex(const fir_filter* filter, fir_state* state, std::uint32_t decimation)
{
    std::complex<float>* input = (std::complex<float>*)state->input;
    std::complex<float>* output = (std::complex<float>*)state->output;

    for(size_t i=0; i < state->output_samples; ++i)
    {
        output[i] = 0.0f;
        size_t input_index = filter->tap_count - 1 + i * decimation;

        for(size_t u=0; u < filter->tap_count; ++u)
        {
            output[i] += input[input_index - filter->tap_count + 1 + u] * filter->array[u];
        }
    }

    size_t target_input_index = filter->tap_count - 1 + state->output_samples * decimation;
    size_t last_index = state->input_samples - 1;
    size_t diffy = target_input_index - last_index - 1;

    state->input_offset = filter->tap_count - 1 - diffy;
    size_t copy_start_offset = state->input_samples - state->input_offset;
    memmove(input, input + copy_start_offset, (state->input_offset) * sizeof(std::complex<float>));
}

static void decimate_real(const fir_filter* filter, fir_state* state, std::uint32_t decimation)
{
    float* input = (float*)state->input;
    float* output = (float*)state->output;

    for(size_t i=0; i < state->output_samples; ++i)
    {
        output[i] = 0.0f;
        size_t input_index = i * decimation;

        for(size_t u=0; u < filter->tap_count; ++u)
        {
            output[i] += input[input_index + u] * filter->array[u];
        }
    }

    size_t target_input_index = filter->tap_count - 1 + state->output_samples * decimation;
    size_t last_index = state->input_samples - 1;
    size_t diffy = target_input_index - last_index - 1;

    state->input_offset = filter->tap_count - 1 - diffy;
    size_t copy_start_offset = state->input_samples - state->input_offset;
    memmove(input, input + copy_start_offset, (state->input_offset) * sizeof(float));
}

void fir_filter::decimate(fir_state* state, std::uint32_t decimation, error& err)
{   
    size_t requested_size = (state->output_samples - 1) * decimation + this->tap_count;

    if(requested_size > state->input_samples)
    {
        err.m_bError = true;
        err.m_sErrorMessage = "Cannot decimate, too many output samples requested for this input size and decimation";
        return;
    }

    if(state->data_type == data_type_t::float32)
    {
        decimate_real(this, state, decimation);
    }
    else
    {
        decimate_complex(this, state, decimation);
    }
}
