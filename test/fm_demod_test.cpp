#include "catch_amalgamated.hpp"
#include "dspapple/fm.hpp"
#include <cmath>

TEST_CASE("fmdemod_test")
{
    const size_t ARRAY_SIZE = 1024;

    float input[ARRAY_SIZE+1];
    std::complex<float> samples[ARRAY_SIZE+1];
    float carry = 0;
    for(size_t i=0; i < ARRAY_SIZE+1; ++i)
    {
        input[i] = std::sin(i / (M_PI * 4));
    }

    dspapple::fm_mod(input, samples, ARRAY_SIZE, 1, &carry);
    float output[ARRAY_SIZE+1];
    
    dspapple::fm_demod(samples, output, 513, 1);
    dspapple::fm_demod(samples+512, output+512, 513, 1);

    for(size_t i=0; i < ARRAY_SIZE-1; ++i)
    {
        REQUIRE(std::abs(input[i] - output[i]) < 1e-5);
    }
}
