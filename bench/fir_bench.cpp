#include <benchmark/benchmark.h>
#include "dspapple/fir.hpp"
#include <cstring>
#include <stdexcept>

static void BM_FIR(benchmark::State& state) {
  constexpr size_t ARRAY_SIZE = 4096;
  constexpr size_t DECIMATION = 2;
  constexpr size_t WINDOW_SIZE = 30;
  constexpr size_t OUTPUT_SIZE = ARRAY_SIZE / DECIMATION;
  dspapple::fir_buffer input;
  dspapple::fir_buffer output;
  dspapple::fir_filter filter;
  input.init(ARRAY_SIZE, WINDOW_SIZE, dspapple::data_type_t::complex32);
  output.init(OUTPUT_SIZE, 0, dspapple::data_type_t::complex32);
  filter.init(WINDOW_SIZE);

  for(size_t i=0; i < WINDOW_SIZE; ++i)
  {
    filter.array[i] = 0.25f;
  }
  
  std::memset(input.m_pBuffer, 0, input.m_uBufferSize * sizeof(std::complex<float>));

  for(auto _ : state) {
    auto result = filter.decimate(&input, &output, DECIMATION);
    assert(result == dspapple::error_code::success);
  }
  
  size_t samples_processed = state.iterations() * ARRAY_SIZE;

  state.SetItemsProcessed(samples_processed);
  state.SetBytesProcessed(samples_processed * sizeof(std::complex<float>));
}

BENCHMARK(BM_FIR);
