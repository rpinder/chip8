#include "cpu.hpp"
#include "renderer.hpp"

#include <iostream>
#include <unistd.h>



auto handle_events(SDL_Event& event, unsigned char keymap[16], std::array<bool, 16>& keys) -> void
{
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_KEYDOWN:
            for (int i = 0; i < 16; i++)
                if (event.key.keysym.sym == keymap[i])
                    keys[i] = true;
            break;
        case SDL_KEYUP:
            for (int i = 0; i < 16; i++)
                if (event.key.keysym.sym == keymap[i])
                    keys[i] = false;
            break;
        default:
            std::cerr << "unhandled SDL event: " <<  event.type << std::endl;
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
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

    for (;;) {
        handle_events(event, keymap, keys); 
        cpu.cycle(); 
        if (cpu.drawing())
            renderer.draw();
        usleep(2000);
    }


    return 0;
}

