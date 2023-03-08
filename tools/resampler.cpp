#include <cstdio>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <complex>
#include "dspapple/resample.hpp"
#include "dspapple/io.hpp"

int main(int argc, char** argv)
{
    if(!freopen(NULL, "wb", stdout) || !freopen(NULL, "rb", stdin))
    {
        fprintf(stderr, "freopen failed, error %s\n", strerror(errno));
        return 1;
    }

    if(argc <= 1)
    {
        std::fprintf(stderr, "Usage: resampler <decimation>\n");
        return 0;
    }

    int decimation = std::atoi(argv[1]);

    dspapple::FloatResampler sampler;
    dspapple::error err;
    constexpr size_t MIN_OUTPUT_SIZE = 512;
    sampler.Init(MIN_OUTPUT_SIZE, decimation, 1, err);
    void* input = malloc(sampler.m_uInputBytes);

    while(dspapple::read_from_file(stdin, input, sampler.m_uInputBytes) != 0)
    {
        sampler.Resample((float*)input);

        if(dspapple::write_to_file(stdout, sampler.m_pBuffer, sampler.m_uOutputBytes) == 0)
            break;
    }

    fflush(stdout);
    fclose(stdout);
}