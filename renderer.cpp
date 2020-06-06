#include "renderer.hpp"

#include <iostream>

Renderer::Renderer(Memory& mem) 
: mem(mem)
{}

auto Renderer::draw() -> void
{
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            std::cout << (mem.display[x + (y * 64)] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
}
