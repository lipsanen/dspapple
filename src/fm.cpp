#include "dspapple/fm.hpp"
#include <cmath>
#include <string.h>
#ifdef __AVX2__
#define SSE
#define AVX
#include "simd_utils/simd_utils.h"
#endif

struct float_vec {
    union {
        alignas(64) v8sf vec;
        alignas(64) float values[8];
    };
};

void dspapple::fm_mod(float* input, std::complex<float>* output, size_t samples, int k, float* carry)
{
#ifdef __AVX2__
    float sum = *carry;
    float_vec vec;
    float_vec sins;
    float_vec coss;
    size_t i=0;

    // samples >= 8 in case the subtraction underflows
    for(; i < samples - 8 && samples >= 8; i += 8)
    {
        for(size_t u=0; u < 8; ++u)
        {
            vec.values[u] = k * sum;
            sum += input[i+u];
        }

        sincos256_ps(vec.vec, &sins.vec, &coss.vec);

        for(size_t u=0; u < 8; ++u)
        {
            float real = coss.vec[u] * 0.5f;
            float imag = sins.vec[u] * 0.5f;
            output[i+u] = std::complex<float>(real, imag);
        }
    }

    for(; i < samples; ++i)
    {
        float real = std::cos(k * sum) / 2;
        float imag = std::sin(k * sum) / 2;
        output[i] = std::complex<float>(real, imag);
        sum += input[i];
    }

    sum = *carry;
    *carry = std::fmod<float>(sum, 2 * M_PI);
#else
    float sum = *carry;

    for(size_t i=0; i < samples; ++i)
    {
        float real = std::cos(k * sum) / 2;
        float imag = std::sin(k * sum) / 2;
        output[i] = std::complex<float>(real, imag);
        sum += input[i];
    }

    *carry = std::fmod<float>(sum, 2 * M_PI);
#endif
}

void dspapple::fm_demod(std::complex<float>* input, float* output, size_t samples, int k)
{
#if 0
    // The custom SIMD implementation is slower than what gcc can cook up
    float_vec sins;
    float_vec coss;
    float_vec outputs;
    size_t i=1;

    for(; i < samples - 8; i += 8)
    {
        for(size_t u=0; u < 8; ++u)
        {
            sins.values[u] = input[i-1+u].real() * input[i+u].imag() - input[i+u].real() * input[i-1+u].imag();
            coss.values[u] = input[i+u].real() * input[i-1+u].real() + input[i-1+u].imag() * input[i+u].imag();
        }

        atan2256f(sins.values, coss.values, output + i - 1, 8);
    }

    for(; i < samples; ++i)
    {
        float s_sin = input[i-1].real() * input[i].imag() - input[i].real() * input[i-1].imag();
        float s_cos = input[i].real() * input[i-1].real() + input[i-1].imag() * input[i].imag();
        float g2 = std::atan2(s_sin, s_cos);
        output[i-1] = g2;
    }

    // Shift the last input to first in preparation for the next iteration
    input[0] = input[samples-1];
#else
    for(size_t i=1; i < samples; ++i)
    {
        float s_sin = input[i-1].real() * input[i].imag() - input[i].real() * input[i-1].imag();
        float s_cos = input[i].real() * input[i-1].real() + input[i-1].imag() * input[i].imag();
        float g2 = std::atan2(s_sin, s_cos);
        output[i-1] = g2;
    }

#endif
}
