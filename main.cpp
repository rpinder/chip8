#include "cpu.hpp"

#include <iostream>
#include <unistd.h>

auto main(int argc, char* argv[]) -> int
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    Memory mem;
    SdlRenderer renderer(mem, 10);
    Cpu cpu(mem, renderer);

    SDL_Event* event = new SDL_Event();

    mem.load(argv[1]);

    for (;;) {
        while (SDL_PollEvent(event)) {
            if (event->type == SDL_QUIT) {
                exit(0);
            }
        }
       cpu.cycle(); 
       if (cpu.drawing())
           renderer.draw();
       cpu.setKeys();
       usleep(50);
    }

    delete event;

    return 0;
}
