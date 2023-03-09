#include "dspapple/utils.hpp"
#include "catch_amalgamated.hpp"

TEST_CASE("int16 to float")
{
    int16_t input[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float output[8];
    dspapple::CastSamples(input, output, 8);
    REQUIRE(output[0] == 1.0f);
    REQUIRE(output[7] == 8.0f);
}

TEST_CASE("int16 to complex")
{
    int16_t input[] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::complex<float> output[4];
    dspapple::CastSamples(input, (float*)output, 8);
    REQUIRE(output[0] == std::complex<float>{1.0f, 2.0f});
    REQUIRE(output[3] == std::complex<float>{7.0f, 8.0f});
}
