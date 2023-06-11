#pragma once

#include <complex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include "dspapple/ringbuffer.hpp"

namespace dspapple
{
    template<typename T>
    struct BatchData
    {
        T* data = nullptr;
        std::size_t elements = 0;
    };

    struct BatchSettings
    {
        std::size_t buffers;
        std::size_t batches;
        std::size_t n;
    };

    struct IntToComplexConverterSettings
    {
        std::size_t buffers;
        std::size_t batches;
        std::size_t n;
        float maxValue;
    };

    struct IntToComplexConverter
    {
        void Init(const IntToComplexConverterSettings& settings);
        void DoWork(RingBuffer<BatchData<std::int16_t>>& bufferIn, RingBuffer<BatchData<std::complex<float>>>& bufferOut);

        std::vector<std::unique_ptr<std::complex<float>[]>> m_vecBuffers;
        IntToComplexConverterSettings m_Settings;
    };

    struct SignalToWindowedSettings
    {
        std::size_t buffers; // How many buffers
        std::size_t batches; // How many batches per buffer
        std::size_t advance; // How many samples to advance per iteration (determines the amount of overlap)
        std::size_t n; // How many samples in each batch
        bool useWindowingFunction = true; // If we should multiply the samples by a windowing function
    };

    struct SignalToWindowedConverter
    {
        void Init(const SignalToWindowedSettings& settings);
        void DoWork(RingBuffer<BatchData<std::complex<float>>>& bufferIn, RingBuffer<BatchData<std::complex<float>>>& bufferOut);

        std::vector<std::unique_ptr<std::complex<float>[]>> m_vecBuffers;
        std::unique_ptr<float[]> m_pWindow;
        SignalToWindowedSettings m_Settings;
    };

    struct FFTProducer
    {
        void Init(const BatchSettings& settings);
        void DoWork(RingBuffer<BatchData<std::complex<float>>>& bufferIn, RingBuffer<BatchData<std::complex<float>>>& bufferOut);

        std::vector<std::unique_ptr<std::complex<float>[]>> m_vecBuffers;
        BatchSettings m_Settings;
    };

    struct FFTMagnitudeProducer
    {
        void Init(const BatchSettings& settings);
        void DoWork(RingBuffer<BatchData<std::complex<float>>>& bufferIn, RingBuffer<BatchData<float>>& bufferOut);

        std::vector<std::unique_ptr<float[]>> m_vecBuffers;
        BatchSettings m_Settings;
    };
}
