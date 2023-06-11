#include "gtest/gtest.h"
#include "dspapple/cuda/utils.cuh"

TEST(CUDA, Conversion)
{
    size_t SAMPLES = 1024;
    int16_t* input;
    cufftComplex* output;

    cudaMallocManaged(&input, SAMPLES * sizeof(int16_t));
    ASSERT_EQ(cudaGetLastError(), cudaSuccess) << cudaGetErrorString(cudaGetLastError());

    for(size_t i=0; i < SAMPLES; ++i)
    {
        input[i] = i;
    }

    cudaMallocManaged(&output, (SAMPLES / 2) * sizeof(cufftComplex));
    ASSERT_EQ(cudaGetLastError(), cudaSuccess) << cudaGetErrorString(cudaGetLastError());

    dspapple::ConvertVecGpu(input, output, SAMPLES);
    cudaDeviceSynchronize();

    const float EPS = 1e-5;

    for(size_t i=0; i < SAMPLES; i += 2)
    {
        EXPECT_NEAR(output[i / 2].x, i / 32767.0f, EPS);
        EXPECT_NEAR(output[i / 2].y, (i + 1) / 32767.0f, EPS);
    }

    cudaFree(input);
    cudaFree(output);
}
