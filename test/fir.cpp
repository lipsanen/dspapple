#include "gtest/gtest.h"
#include "dspapple/fir.hpp"

TEST(FIR, Averaging)
{
    {
        float input_arr[] = {0, 0, 1, 1};
        float output_arr[] = {0, 0};

        dspapple::fir_buffer input_buf;
        dspapple::fir_buffer output_buf;
        input_buf.init_ptr(input_arr, 4, 3);
        output_buf.init_ptr(output_arr, 2, 0);

        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        auto result = filter.decimate(&input_buf, &output_buf, 1);
        EXPECT_EQ(result, dspapple::error_code::success);
        EXPECT_TRUE(output_arr[0] == 1.0f);
        EXPECT_TRUE(output_arr[1] == 2.0f);
    }

    {
        std::complex<float> input_arr[] = {0, 0, {1.0f, 2.0f}, {3.0f, 4.0f}};
        std::complex<float> output_arr[] = {0, 0};

        dspapple::fir_buffer input_buf;
        dspapple::fir_buffer output_buf;
        input_buf.init_ptr(input_arr, 4, 3);
        output_buf.init_ptr(output_arr, 2, 0);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        auto result = filter.decimate(&input_buf, &output_buf, 1);
        EXPECT_EQ(result, dspapple::error_code::success);
        EXPECT_TRUE(output_arr[0] == std::complex<float>(1.0f, 2.0f));
        EXPECT_TRUE(output_arr[1] == std::complex<float>(4.0f, 6.0f));
    }

}

TEST(FIR, Decimate)
{
    {
        float input[] = {0, 0, 1, 1, 1, 1};
        float output[] = {0, 0};

        dspapple::fir_buffer input_buf;
        dspapple::fir_buffer output_buf;
        input_buf.init_ptr(input, 6, 3);
        output_buf.init_ptr(output, 2, 0);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        auto result = filter.decimate(&input_buf, &output_buf, 2);
        EXPECT_EQ(result, dspapple::error_code::success);
        EXPECT_EQ(output[0], 1.0f);
        EXPECT_EQ(output[1], 3.0f);
        EXPECT_TRUE(input[0] == 1.0f);
        EXPECT_TRUE(input[1] == 1.0f);
    }
}
