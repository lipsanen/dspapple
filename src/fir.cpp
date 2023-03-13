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

fir_buffer::~fir_buffer()
{
    deallocate();
}

void fir_buffer::init(std::uint32_t samples, std::uint32_t window_size, data_type_t data_type)
{
    if(window_size <= 0)
    {
        m_uBufferSize = samples;
    }
    else
    {
        m_uBufferSize = samples + window_size - 1;
    }

    m_eDatatype = data_type;
    size_t sample_size = (m_eDatatype == data_type_t::float32) ? 4 : 8;
    m_pBuffer = malloc(sample_size * m_uBufferSize);
    m_bAllocated = true;
    m_uSamples = samples;
}

void fir_buffer::init_ptr(float* input, std::uint32_t array_size, std::uint32_t window_size)
{
    m_eDatatype = data_type_t::float32;
    m_pBuffer = input;
    m_uBufferSize = array_size;
    m_bAllocated = false;
    if(window_size > 1)
    {
        m_uSamples = array_size - window_size + 1;
    }
    else
    {
        m_uSamples = array_size;
    }
}

void fir_buffer::init_ptr(std::complex<float>* input, std::uint32_t array_size, std::uint32_t window_size)
{
    m_eDatatype = data_type_t::complex32;
    m_pBuffer = input;
    m_uBufferSize = array_size;
    m_bAllocated = false;
    if(window_size > 1)
    {
        m_uSamples = array_size - window_size + 1;
    }
    else
    {
        m_uSamples = array_size;
    }
}

void fir_buffer::deallocate()
{
    if(m_bAllocated)
    {
        free(m_pBuffer);
        m_pBuffer = nullptr;
        m_bAllocated = false;
    }
}

void* fir_buffer::get_input_dest()
{
    if(m_eDatatype == data_type_t::float32)
    {
        float* out = (float*)m_pBuffer;
        return out + (m_uBufferSize - m_uSamples);
    }
    else
    {
        std::complex<float>* out = (std::complex<float>*)m_pBuffer;
        return out + (m_uBufferSize - m_uSamples);
    }
}

std::size_t fir_buffer::get_input_bytes()
{
    if(m_eDatatype == data_type_t::float32)
    {
        return m_uSamples * 4;
    }
    else
    {
        return m_uSamples * 8;
    }
}

std::size_t fir_buffer::get_window_size()
{
    return (m_uBufferSize - m_uSamples) + 1;
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

template<typename T>
static void _decimate(const fir_filter* filter, fir_buffer* input, fir_buffer* output, std::uint32_t decimation)
{
    auto* input_buffer = (T*)input->get_input_dest();
    auto* output_buffer = (T*)output->get_input_dest();

    for(size_t i=0; i < output->m_uSamples; ++i)
    {
        output_buffer[i] = 0.0f;
        size_t input_index = i * decimation;

        for(size_t u=0; u < filter->tap_count; ++u)
        {
            output_buffer[i] += input_buffer[input_index - filter->tap_count + 1 + u] * filter->array[u];
        }
    }

    size_t copy_start_offset = input->m_uBufferSize - filter->tap_count;
    memmove(input->m_pBuffer, ((T*)input->m_pBuffer) + copy_start_offset, (filter->tap_count) * sizeof(T));
}

error_code fir_filter::decimate(fir_buffer* input, fir_buffer* output, std::uint32_t decimation)
{   
    if(((double)input->m_uSamples / decimation) != output->m_uSamples)
    {
        return error_code::fir_input_output_mismatch;
    }

    if(input->m_eDatatype != output->m_eDatatype)
    {
        return error_code::fir_input_output_mismatch;
    }

    if((double)input->get_window_size() != tap_count)
    {
        return error_code::fir_input_window_size_doesnt_match_filter;
    }

    if(input->m_eDatatype == data_type_t::float32)
    {
        _decimate<float>(this, input, output, decimation);
    }
    else
    {
        _decimate<std::complex<float>>(this, input, output, decimation);
    }

    return error_code::success;
}


fir_filter::~fir_filter()
{
    if(this->array)
    {
        free(this->array);
    }
}
