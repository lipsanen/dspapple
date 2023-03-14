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
        filter.init(3, dspapple::window_type::none);
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
        filter.init(3, dspapple::window_type::none);
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
        filter.init(3, dspapple::window_type::none);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        auto result = filter.decimate(&input_buf, &output_buf, 2);
        EXPECT_EQ(result, dspapple::error_code::success);
        EXPECT_EQ(output[0], 1.0f);
        EXPECT_EQ(output[1], 3.0f);
        EXPECT_TRUE(input[0] == 1.0f);
        EXPECT_TRUE(input[1] == 1.0f);
    }
}

TEST(FIR, Convolution)
{
    float arr[] = {0.02455383, 0.23438946, 0.4821134 , 0.23438946, 0.02455383};
    dspapple::fir_buffer input_buf;
    dspapple::fir_buffer output_buf;
    input_buf.init(32, 5, dspapple::data_type_t::float32);
    output_buf.init(32, 0, dspapple::data_type_t::float32);
    dspapple::fir_filter filter;
    filter.init(5, dspapple::window_type::none);
    memcpy(filter.array, arr, sizeof(arr));

    float* input = (float*)input_buf.get_input_dest();

    for(size_t i=0; i < 32; ++i)
    {
        input[i] = i;
    }

    filter.decimate(&input_buf, &output_buf, 1);

    float* output = (float*)output_buf.get_input_dest();

    ASSERT_NEAR(output[0], 0, 1e-4);
    ASSERT_NEAR(output[1], 0.024554, 1e-4);
    ASSERT_NEAR(output[2], 0.283497, 1e-4);
    ASSERT_NEAR(output[3], 1.024554, 1e-4);
    ASSERT_NEAR(output[4], 2, 1e-4);
    ASSERT_NEAR(output[10], 8, 1e-4);
    ASSERT_NEAR(output[20], 18, 1e-4);
}
