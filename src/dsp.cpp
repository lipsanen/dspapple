#include "dspapple/dsp.hpp"

using namespace dspapple;

void IntToComplexConverter::Init(const IntToComplexConverterSettings& settings)
{
    m_vecBuffers.clear();
    for(size_t i=0; i < settings.buffers; ++i)
    {
        m_vecBuffers.push_back(
            std::unique_ptr<std::complex<float>[]>(new std::complex<float>[settings.n * settings.batches])
        );
    }
    m_Settings = settings;
}

void IntToComplexConverter::DoWork(RingBuffer<BatchData<std::int16_t>>& bufferIn, RingBuffer<BatchData<std::complex<float>>>& bufferOut)
{
    auto inputData = bufferIn.front();
    std::size_t bufferIndex = 0;

    while(inputData.data)
    {
        BatchData<std::complex<float>> output;
        output.data = m_vecBuffers[bufferIndex].get();
        output.elements = inputData.elements / 2;

        for(size_t i=0; i < inputData.elements; i += 2)
        {
            float re = inputData.data[i] / m_Settings.maxValue;
            float im = inputData.data[i+1] / m_Settings.maxValue;

            output.data[i/2] = std::complex<float>(re, im);
        }

        ++bufferIndex;
        if(bufferIndex == m_vecBuffers.size())
            bufferIndex = 0;
        bufferIn.pop();
        inputData = bufferIn.front();
    }

    bufferIn.pop();
    bufferOut.push(BatchData<std::complex<float>>());
}
