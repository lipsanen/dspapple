#include "dspapple/fir.hpp"
#include <string.h>

using namespace dspapple;

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

void fir_filter::init(std::uint32_t taps)
{
    this->tap_count = taps;
    this->array = (float*)malloc(taps * sizeof(float));
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
            output[i] += input[input_index - u] * filter->array[u];
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
        size_t input_index = filter->tap_count - 1 + i * decimation;

        for(size_t u=0; u < filter->tap_count; ++u)
        {
            output[i] += input[input_index - u] * filter->array[u];
        }
    }

    size_t target_input_index = filter->tap_count - 1 + state->output_samples * decimation;
    size_t last_index = state->input_samples - 1;
    size_t diffy = target_input_index - last_index - 1;

    state->input_offset = filter->tap_count - 1 - diffy;
    size_t copy_start_offset = state->input_samples - state->input_offset;
    memmove(input, input + copy_start_offset, (state->input_offset) * sizeof(float));
}

void fir_filter::decimate(fir_state* state, std::uint32_t decimation)
{   
    if(state->data_type == data_type_t::float32)
    {
        decimate_real(this, state, decimation);
    }
    else
    {
        decimate_complex(this, state, decimation);
    }
}
