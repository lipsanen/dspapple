/*
Modified version of the SPSCQueue by rigtorp: https://github.com/rigtorp/SPSCQueue
 */

#pragma once

#include <atomic>
#include <cstddef>

namespace dspapple
{
    constexpr std::size_t YIELD_AFTER_ITERATIONS = 10000;
    static constexpr size_t kCacheLineSize = 64;

    template <typename T>
    struct AlignedValue
    {
        alignas(kCacheLineSize) T value;
    };

    template <typename T>
    struct RingBuffer
    {
        RingBuffer(std::size_t capacity) {
            capacity_ = capacity + 1;
            slots_ = new AlignedValue<int>[capacity_];
        }

        ~RingBuffer() {
            delete[] slots_;
        }

        // non-copyable and non-movable
        RingBuffer(const RingBuffer &) = delete;
        RingBuffer &operator=(const RingBuffer &) = delete;

        void push(T v)
        {
            auto const writeIdx = writeIdx_.load(std::memory_order_relaxed);
            auto nextWriteIdx = writeIdx + 1;
            if (nextWriteIdx == capacity_)
            {
                nextWriteIdx = 0;
            }
            for (size_t i = 0; nextWriteIdx == readIdxCache_; ++i)
            {
                if (i >= YIELD_AFTER_ITERATIONS)
                    std::this_thread::yield();
                readIdxCache_ = readIdx_.load(std::memory_order_acquire);
            }
            slots_[writeIdx].value = v;
            writeIdx_.store(nextWriteIdx, std::memory_order_release);
        }

        T front() noexcept
        {
            auto const readIdx = readIdx_.load(std::memory_order_relaxed);
            if (readIdx == writeIdxCache_)
            {
                for(std::size_t i=0; writeIdxCache_ == readIdx; ++i)
                {
                    writeIdxCache_ = writeIdx_.load(std::memory_order_acquire);
                }
            }
            return slots_[readIdx].value;
        }

        void pop(bool checked=true) noexcept
        {
            if(checked)
            {
                auto value = front();
            }
        
            auto const readIdx = readIdx_.load(std::memory_order_relaxed);
            auto nextReadIdx = readIdx + 1;
            if (nextReadIdx == capacity_)
            {
                nextReadIdx = 0;
            }
            readIdx_.store(nextReadIdx, std::memory_order_release);
        }

        std::size_t capacity_;
        AlignedValue<T>* slots_;

        // Align to cache line size in order to avoid false sharing
        // readIdxCache_ and writeIdxCache_ is used to reduce the amount of cache
        // coherency traffic
        alignas(kCacheLineSize) std::atomic<size_t> writeIdx_ = {0};
        alignas(kCacheLineSize) size_t readIdxCache_ = 0;
        alignas(kCacheLineSize) std::atomic<size_t> readIdx_ = {0};
        alignas(kCacheLineSize) size_t writeIdxCache_ = 0;

        // Padding to avoid adjacent allocations to share cache line with
        // writeIdxCache_
        char padding_[kCacheLineSize - sizeof(writeIdxCache_)];
    };
} // namespace rigtorp
