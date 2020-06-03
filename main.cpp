#include "cpu.hpp"

auto main(int argc, char **argv) -> int
{
    Memory mem;
    Renderer renderer;
    Cpu cpu(mem, renderer);

    mem.load("game");

    for(;;) {
       cpu.cycle(); 
       if (cpu.drawing())
           renderer.draw();
       cpu.setKeys();
    }

    return 0;
}
