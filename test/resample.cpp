#include "catch_amalgamated.hpp"
#include "dspapple/resample.hpp"
#include <cmath>

TEST_CASE("resample arrays")
{
    dspapple::FloatResampler sampler;
    dspapple::error err;
    sampler.Init(1024, 2, 1, err);
    REQUIRE(sampler.m_uDecimation == 2);
    REQUIRE(sampler.m_uInterpolation == 1);
    REQUIRE(sampler.m_uInputBytes == 2048);
    REQUIRE(sampler.m_uOutputBytes == 1024);

    dspapple::FloatResampler sampler2;
    sampler2.Init(1024, 1, 2, err);
    REQUIRE(sampler2.m_uDecimation == 1);
    REQUIRE(sampler2.m_uInterpolation == 2);
    REQUIRE(sampler2.m_uInputBytes == 512);
    REQUIRE(sampler2.m_uOutputBytes == 1024);
}

TEST_CASE("basic resampling")
{
    dspapple::FloatResampler sampler;
    dspapple::error err;
    sampler.Init(3 * sizeof(float), 2, 1, err);
    REQUIRE(sampler.m_uDecimation == 2);
    REQUIRE(sampler.m_uInterpolation == 1);
    REQUIRE(sampler.m_uInputBytes == 8 * sizeof(float));
    REQUIRE(sampler.m_uOutputBytes == 4 * sizeof(float));

    float input[] = {0, 1, 2, 3, 4, 5, 6, 7};
    sampler.Resample(input);
    REQUIRE(sampler.m_pBuffer[0] == 0.0f);
    REQUIRE(sampler.m_pBuffer[1] == 2.0f);
    REQUIRE(sampler.m_pBuffer[2] == 4.0f);
    REQUIRE(sampler.m_pBuffer[3] == 6.0f);
}
