#include <benchmark/benchmark.h>
#include "dspapple/utils.hpp"
#include <cstring>

static void BM_CPU_Int16F32(benchmark::State& state) {
  constexpr size_t SAMPLES = 128;
  int16_t samples[SAMPLES];
  float output[SAMPLES];
  for(size_t i=0; i < SAMPLES; ++i)
  {
    samples[i] = i;
  }

  for(auto _ : state) {
    dspapple::CastSamples(samples, output, SAMPLES);
    benchmark::DoNotOptimize(output);
  }

  state.SetItemsProcessed(SAMPLES * state.iterations());
  state.SetBytesProcessed(SAMPLES * state.iterations() * sizeof(int16_t));
}


static void BM_CPU_Int16C32(benchmark::State& state) {
  constexpr size_t SAMPLES = 128;
  int16_t samples[SAMPLES];
  std::complex<float> output[64];

  for(size_t i=0; i < SAMPLES; ++i)
  {
    samples[i] = i;
  }

  for(auto _ : state) {
    dspapple::CastSamples(samples, (float*)output, SAMPLES);
    benchmark::DoNotOptimize(output);
  }

  state.SetItemsProcessed(SAMPLES * state.iterations());
  state.SetBytesProcessed(SAMPLES* state.iterations() * sizeof(int16_t));
}

BENCHMARK(BM_CPU_Int16C32);
BENCHMARK(BM_CPU_Int16F32);
