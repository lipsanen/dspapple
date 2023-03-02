#pragma once

#include <complex>
#include <stddef.h>

namespace dsp_apple
{
    // Modulate signal into an FM baseband signal
    void fm_mod(float* input, std::complex<float>* output, size_t samples, int k, float* carry);
    // Demodulate FM from baseband signal
    void fm_demod(std::complex<float>* input, float* output, size_t samples, int k);
}

