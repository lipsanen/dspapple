#include <SDL2/SDL_audio.h>
#include <SDL2/SDL.h>
#include <atomic>
#include <cstdio>
#include <thread>

struct data {
    bool finished = false;
};

void callback(void* userdata, Uint8* _stream, int len) {
    data* d = (data*)userdata;
    float* stream = (float*)_stream;
    int offset = 0;

    while(!d->finished && offset < len)
    {
        int bytesToRead = len - offset;
        auto result = fread(_stream + offset, 1, bytesToRead, stdin);
        offset += result;

        if(result <= 0)
        {
            d->finished = true;
        }
    }

    for(int i=offset / sizeof(float); i < len / sizeof(float); ++i)
    {
        stream[i] = 0.0f;
    }

}

int main()
{
    if(!freopen(NULL, "rb", stdin))
    {
        fprintf(stderr, "freopen failed, error %s\n", strerror(errno));
        return 1;
    }

    if(SDL_Init(SDL_INIT_AUDIO) < 0)
        return 1;
    data d;
    SDL_AudioSpec want;
    SDL_zero(want);
    want.callback = callback;
    want.userdata = &d;
    want.freq = 48000;
    want.format = AUDIO_F32;
    want.channels = 1;
    want.samples = 48000;

    if(SDL_OpenAudio(&want, NULL) < 0) {
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  exit(-1);
	}

    SDL_PauseAudio(0);

    while(!d.finished)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    SDL_CloseAudio();
}