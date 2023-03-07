#include <benchmark/benchmark.h>
#include "dspapple/fm.hpp"
#include <cstring>

static void BM_DemodFM(benchmark::State& state) {
  std::complex<float> samples[1025];
  float output[1024];
  std::memset(samples, 0, sizeof(samples));

  for(auto _ : state) {
    dspapple::fm_demod(samples, output, 1025, 1);
  }

  state.SetItemsProcessed(1024 * state.iterations());
  state.SetBytesProcessed(1024 * state.iterations() * sizeof(std::complex<float>));
}

static void BM_ModFM(benchmark::State& state) {
  std::complex<float> output[1024];
  float input[1024];
  float carry;
  std::memset(input, 0, sizeof(input));

  for(auto _ : state) {
    dspapple::fm_mod(input, output, 1024, 1, &carry);
  }

  state.SetItemsProcessed(1024 * state.iterations());
  state.SetBytesProcessed(1024 * state.iterations() * sizeof(std::complex<float>));
}

BENCHMARK(BM_DemodFM);
BENCHMARK(BM_ModFM);
BENCHMARK_MAIN();
