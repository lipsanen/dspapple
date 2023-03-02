#include <cstdio>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
    if(!freopen(NULL, "wb", stdout))
    {
        fprintf(stderr, "freopen failed, error %s\n", strerror(errno));
        return 1;
    }

    float duration = std::atof(argv[1]);
    int samples = duration * 48000;

    for(int i=0; i < samples; ++i)
    {
        float f = std::sin(i / (M_PI * 32)) * 0.5;
        fwrite(&f, 1, sizeof(f), stdout);
    }

    fflush(stdout);
    fclose(stdout);
}