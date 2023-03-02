#include <cstdio>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <complex>
#include "dspapple/fm.hpp"
#include "dspapple/io.hpp"

int main(int argc, char** argv)
{
    if(!freopen(NULL, "wb", stdout) || !freopen(NULL, "rb", stdin))
    {
        fprintf(stderr, "freopen failed, error %s\n", strerror(errno));
        return 1;
    }

    constexpr size_t OUTPUT_SIZE = 512;

    std::complex<float> input[OUTPUT_SIZE+1];
    float output[OUTPUT_SIZE];

    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(output));

    while(dspapple::read_from_file(stdin, input+1, OUTPUT_SIZE * sizeof(std::complex<float>)) != 0)
    {
        dsp_apple::fm_demod(input, output, OUTPUT_SIZE+1, 1);

        for(size_t i=0; i < OUTPUT_SIZE; ++i)
        {
            output[i] *= 0.025f;
        }

        if(dspapple::write_to_file(stdout, output, sizeof(output)) == 0)
            break;
    }

    fflush(stdout);
    fclose(stdout);
}