#pragma once

#include <cstdint>
#include "dspapple/utils.hpp"

namespace dspapple
{
    struct FloatResampler
    {
        std::uint16_t m_uDecimation = 1;
        std::uint16_t m_uInterpolation = 1;
        std::uint32_t m_uInputBytes = 1024;
        std::uint32_t m_uOutputBytes = 1024;
        float* m_pBuffer = nullptr;

        FloatResampler() = default;
        ~FloatResampler();
        void Resample(float* input);
        void Init(std::size_t minOutputSize, std::uint16_t decimation, std::uint16_t interpolation, Error& err);
    };
}
