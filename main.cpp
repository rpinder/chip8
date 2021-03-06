#include "cpu.hpp"
#include "renderer.hpp"

#include <iostream>
#include <unistd.h>
#include <chrono>



auto handle_events(SDL_Event& event, unsigned char keymap[16], std::array<bool, 16>& keys) -> void
{
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        exit(1);

                    for (int i = 0; i < 16; i++)
                        if (event.key.keysym.sym == keymap[i])
                            keys[i] = true;
                }
                break;
            case SDL_KEYUP:
                for (int i = 0; i < 16; i++)
                    if (event.key.keysym.sym == keymap[i])
                        keys[i] = false;
                break;
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    if (argc < 2) {
        std::cout << "Enter rom as second argument" << std::endl;
        exit(1);
    }

    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << std::endl; 
        exit(1);
    }

    unsigned char keymap[16] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };

    std::array<bool, 16> keys;
    std::fill(keys.begin(), keys.end(), false);

    Memory mem;
    SdlRenderer renderer(mem, 10);
    Cpu cpu(mem, keys);

    SDL_Event event;

    mem.load(argv[1]);

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("beep.wav", &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    for (;;) {
        auto start = std::chrono::high_resolution_clock::now();

        handle_events(event, keymap, keys); 

        cpu.cycle(); 

        if (cpu.should_draw())
            renderer.draw();

        if (cpu.should_beep()) {
            SDL_PauseAudioDevice(deviceId, 0);
            SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        } else {
            SDL_PauseAudioDevice(deviceId, 1);
        }

        auto end = std::chrono::high_resolution_clock::now();
        int duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        int time_sleep = (1000000/540) - duration;
        if (time_sleep < 0) time_sleep = 0;
        usleep(time_sleep);
    }

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();

    return 0;
}

