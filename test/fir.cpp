#include "gtest/gtest.h"
#include "dspapple/fir.hpp"

TEST(FIR, Averaging)
{
    dspapple::error err;
    {
        float input[] = {0, 0, 1, 1};
        float output[] = {0, 0};

        dspapple::fir_state state;
        state.init(input, 4, output, 2);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 1, err);
        EXPECT_TRUE(err.m_bError == false);
        EXPECT_TRUE(output[0] == 1.0f);
        EXPECT_TRUE(output[1] == 2.0f);
        EXPECT_TRUE(state.input_offset == 2);
    }

    {
        std::complex<float> input[] = {0, 0, {1.0f, 2.0f}, {3.0f, 4.0f}};
        std::complex<float> output[] = {0, 0};

        dspapple::fir_state state;
        state.init(input, 4, output, 2);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 1, err);
        EXPECT_TRUE(err.m_bError == false);
        EXPECT_TRUE(output[0] == std::complex<float>(1.0f, 2.0f));
        EXPECT_TRUE(output[1] == std::complex<float>(4.0f, 6.0f));
        EXPECT_TRUE(state.input_offset == 2);
    }

}

TEST(FIR, Decimate)
{
    dspapple::error err;
    {
        float input[] = {0, 0, 1};
        float output[] = {0};

        dspapple::fir_state state;
        state.init(input, 3, output, 1);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 2, err);
        EXPECT_TRUE(err.m_bError == false);
        EXPECT_TRUE(output[0] == 1.0f);
        EXPECT_TRUE(state.input_offset == 1);
        EXPECT_TRUE(input[0] == 1.0f);
    }

    {
        std::complex<float> input[] = {0, 0, {1, 1}};
        std::complex<float> output[] = {0};

        dspapple::fir_state state;
        state.init(input, 3, output, 1);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 2, err);
        EXPECT_TRUE(err.m_bError == false);
        EXPECT_TRUE(output[0] == std::complex<float> (1.0f, 1.0f));
        EXPECT_TRUE(state.input_offset == 1);
        EXPECT_TRUE(input[0].real() == 1.0f);
        EXPECT_TRUE(input[0].real() == 1.0f);
    }

    {
        float input[] = {0, 0, 1, 1};
        float output[] = {0};

        dspapple::fir_state state;
        state.init(input, 4, output, 1);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 2, err);
        EXPECT_TRUE(err.m_bError == false);
        EXPECT_TRUE(output[0] == 1.0f);
        EXPECT_TRUE(state.input_offset == 2);
        EXPECT_TRUE(input[0] == 1.0f);
        EXPECT_TRUE(input[1] == 1.0f);
    }
}
