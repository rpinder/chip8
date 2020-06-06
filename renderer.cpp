#include "renderer.hpp"

#include <iostream>

Renderer::Renderer(Memory& mem) 
    : mem(mem)
{}


TerminalRenderer::TerminalRenderer(Memory &mem)
    : Renderer(mem)
{}

auto TerminalRenderer::draw() const -> void
{
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            std::cout << (mem.display[x + (y * 64)] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
}

SdlRenderer::SdlRenderer(Memory &mem)
    : Renderer(mem)
{}

auto SdlRenderer::draw() const -> void
{

}
