#include "catch_amalgamated.hpp"
#include "dspapple/fir.hpp"

TEST_CASE("FIR averaging")
{
    {
        float input[] = {0, 0, 1, 1};
        float output[] = {0, 0};

        dspapple::fir_state state;
        state.init(input, 4, output, 2);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 1);
        REQUIRE(output[0] == 1.0f);
        REQUIRE(output[1] == 2.0f);
        REQUIRE(state.input_offset == 2);
    }

    {
        std::complex<float> input[] = {0, 0, {1.0f, 2.0f}, {3.0f, 4.0f}};
        std::complex<float> output[] = {0, 0};

        dspapple::fir_state state;
        state.init(input, 4, output, 2);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 1);
        REQUIRE(output[0] == std::complex<float>{1.0f, 2.0f});
        REQUIRE(output[1] == std::complex<float>{4.0f, 6.0f});
        REQUIRE(state.input_offset == 2);
    }

}

TEST_CASE("FIR decimate")
{
    {
        float input[] = {0, 0, 1};
        float output[] = {0};

        dspapple::fir_state state;
        state.init(input, 3, output, 1);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 2);
        REQUIRE(output[0] == 1.0f);
        REQUIRE(state.input_offset == 1);
        REQUIRE(input[0] == 1.0f);
    }

    {
        std::complex<float> input[] = {0, 0, {1, 1}};
        std::complex<float> output[] = {0};

        dspapple::fir_state state;
        state.init(input, 3, output, 1);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 2);
        REQUIRE(output[0] == std::complex<float> {1.0f, 1.0f});
        REQUIRE(state.input_offset == 1);
        REQUIRE(input[0].real() == 1.0f);
        REQUIRE(input[0].real() == 1.0f);
    }

    {
        float input[] = {0, 0, 1, 1};
        float output[] = {0};

        dspapple::fir_state state;
        state.init(input, 4, output, 1);
        dspapple::fir_filter filter;
        filter.init(3);
        filter.array[0] = filter.array[1] = filter.array[2] = 1.0f;
        filter.decimate(&state, 2);
        REQUIRE(output[0] == 1.0f);
        REQUIRE(state.input_offset == 2);
        REQUIRE(input[0] == 1.0f);
        REQUIRE(input[1] == 1.0f);
    }
}
