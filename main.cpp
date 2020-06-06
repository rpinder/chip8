#include "cpu.hpp"

#include <iostream>
#include <unistd.h>

auto main(int argc, char* argv[]) -> int
{
    Memory mem;
    TerminalRenderer renderer(mem);
    Cpu cpu(mem, renderer);

    mem.load(argv[1]);

    for(;;) {
       cpu.cycle(); 
       if (cpu.drawing() && argc > 2)
           renderer.draw();
       cpu.setKeys();
       std::getchar();
    }

    return 0;
}
