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

SdlRenderer::SdlRenderer(Memory &mem, const int scale)
    : Renderer(mem)
{
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64 * scale, 32 * scale, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, 64, 32);
}

SdlRenderer::~SdlRenderer()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

auto SdlRenderer::draw() const -> void
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (mem.display[x + (y * 64)]) {
                SDL_Rect rect;
                rect.x = x;
                rect.y = y;
                rect.w = 1;
                rect.h = 1;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
