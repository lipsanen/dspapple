#pragma once

#include <cuda_runtime_api.h>
#include <cufft.h>

__global__ void VecConvert(int16_t* A, cufftComplex* out, int arrayCount)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < arrayCount)
    {
        out[i/2].x = __uint2float_rn(A[i]) / 32767.0f;
        out[i/2].y = __uint2float_rn(A[i + 1]) / 32767.0f;
    }
}

namespace dspapple
{
    static inline void ConvertVecGpu(int16_t* input, cufftComplex* output, int arrayCount)
    {
        VecConvert<<<32,arrayCount/32>>>(input, output, arrayCount);
    }
}

