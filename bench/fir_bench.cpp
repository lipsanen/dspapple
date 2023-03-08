#include <benchmark/benchmark.h>
#include "dspapple/fir.hpp"
#include <cstring>
#include <stdexcept>

static void BM_FIR(benchmark::State& state) {
  constexpr size_t ARRAY_SIZE = 4096;
  constexpr size_t DECIMATION = 2;
  constexpr size_t WINDOW_SIZE = 30;
  constexpr size_t OUTPUT_SIZE = ARRAY_SIZE / DECIMATION - WINDOW_SIZE;
  dspapple::fir_state fir_state;
  dspapple::fir_filter filter;
  std::complex<float> samples[ARRAY_SIZE];
  std::complex<float> output[OUTPUT_SIZE];
  fir_state.init(samples, ARRAY_SIZE, output, OUTPUT_SIZE);
  filter.init(WINDOW_SIZE);

  for(size_t i=0; i < WINDOW_SIZE; ++i)
  {
    filter.array[i] = 0.25f;
  }
  
  std::memset(samples, 0, sizeof(samples));
  size_t samples_processed = 0;
  dspapple::error err;

  for(auto _ : state) {
    filter.decimate(&fir_state, DECIMATION, err);
    samples_processed += ARRAY_SIZE - fir_state.input_offset;
    if(err.m_bError)
    {
      abort();
    }
  }

  state.SetItemsProcessed(samples_processed);
  state.SetBytesProcessed(samples_processed * sizeof(std::complex<float>));
}

BENCHMARK(BM_FIR);
