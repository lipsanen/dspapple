#include "dspapple/resample.hpp"
#include <cmath>

using namespace dspapple;

void FloatResampler::Init(std::size_t minOutputSize, std::uint16_t decimation, std::uint16_t interpolation, Error& err)
{
    if(m_pBuffer != nullptr)
    {
        err.m_bError = true;
        err.m_sErrorMessage = "Resampler already initialized";
        return;
    }

    m_uOutputBytes = interpolation * sizeof(float);

    while(m_uOutputBytes < minOutputSize)
    {
        m_uOutputBytes *= 2;
    }

    m_uInputBytes = (m_uOutputBytes / interpolation) * decimation;
    m_uDecimation = decimation;
    m_uInterpolation = interpolation;
    m_pBuffer = (float*)malloc(m_uOutputBytes);
}

void FloatResampler::Resample(float* input)
{
    float ratio = ((float)m_uDecimation) / m_uInterpolation;

    for(size_t i=0; i < m_uOutputBytes / sizeof(float); ++i)
    {
        float index = ratio * i;
        std::uint32_t index1 = index;
        std::uint32_t index2 = index1 + 1;
        float output = (1 - (index - index1)) * input[index1] + (index - index1) * input[index2];
        m_pBuffer[i] = output;
    }
}

FloatResampler::~FloatResampler()
{
    free(m_pBuffer);
}