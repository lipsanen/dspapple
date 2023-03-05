#include "dspapple/fm.hpp"
#include <cmath>
#include <string.h>

void dspapple::fm_mod(float* input, std::complex<float>* output, size_t samples, int k, float* carry)
{
    float sum = *carry;

    for(size_t i=0; i < samples; ++i)
    {
        float real = std::cos(k * sum) / 2;
        float imag = std::sin(k * sum) / 2;
        output[i] = std::complex<float>(real, imag);
        sum += input[i];
    }

    *carry = std::fmod<float>(sum, 2 * M_PI);
}

void dspapple::fm_demod(std::complex<float>* input, float* output, size_t samples, int k)
{
    const float samp_rate = 1.0f; // this is some relative sample rate
    float T = 1 / samp_rate;
    float factor = 1.0f / (T * k);

    for(size_t i=1; i < samples; ++i)
    {
        float s_sin = input[i-1].real() * input[i].imag() - input[i].real() * input[i-1].imag();
        float s_cos = input[i].real() * input[i-1].real() + input[i-1].imag() * input[i].imag();
        float g2 = std::atan2(s_sin, s_cos);
        output[i-1] = factor * g2;
    }

    // Shift the last input to first in preparation for the next iteration
    input[0] = input[samples-1];
}
