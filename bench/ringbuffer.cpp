#include <benchmark/benchmark.h>
#include <thread>
#include "dspapple/ringbuffer.hpp"


static void BM_SPSC(benchmark::State& state)
{
    constexpr std::size_t SLOTS = 10;
    std::atomic<bool> ready(false);
    std::atomic<bool> stopped(false);
    dspapple::RingBuffer<int, SLOTS> queue;

    std::thread pusher([&](){
        ready = true;
        int ptr = queue.front();

        while(ptr != 0)
        {
            queue.pop(false);
            ptr = queue.front();
        }
    });

    while(!ready);

    for(auto _ : state)
    {
        queue.push(1);
    }

    queue.push(0);
    state.SetItemsProcessed(state.iterations());
    pusher.join();
}

BENCHMARK(BM_SPSC);
