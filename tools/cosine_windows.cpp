#include <cstdint>
#include <cmath>
#include <cstdio>

static constexpr float EPS = 1e-5;

static float sinc(float x)
{
    x *= M_PI;
    if(std::abs(x) < EPS)
    {
        // sinx / x approaches 1 as x goes near 0, define value as 1 near 0
        return 1;
    }
    else
    {
        return std::sin(x) / x;
    }
}

static void get_bandpass_sinc(float* array, std::int32_t size, float left, float right)
{
    float alpha = 0.5 * (size - 1);
    for(std::int32_t i=0; i < size; ++i)
    {
        float m = i - alpha;
        array[i] = right * sinc(right * m) - left * sinc(left * m);
    }
}

static void apply_cosine(float* array, std::uint32_t count, float A, float B, float C)
{
    float total = 0;
    for(size_t i=0; i < count; ++i)
    {
        float ind = i * M_PI * 2 / (count - 1);
        float val = A - B * std::cos(ind) + C * std::cos(2 * ind);
        array[i] *= val;
    }
}

static void freq_normalize(float* array, std::uint32_t count, float left, float right)
{
    float total = 0;

    for(size_t i=0; i < count; ++i)
    {
        total += array[i];
    }

    if(std::abs(total) < EPS)
    {
        return;
    }

    for(size_t i=0; i < count; ++i)
    {
        array[i] /= total;
    }
}

int main()
{
    float array[5];
    get_bandpass_sinc(array, 5, 0, 0.5);
    apply_cosine(array, 5, 0.54, 0.46, 0);
    freq_normalize(array, 5, 0, 0.5);

    for(size_t i=0; i < 5; ++i)
    {
        printf("%lu: %f\n", i, array[i]);
    }
}