#include "memory.hpp"
#include <SDL2/SDL.h>

class Renderer
{
    public:
        Renderer(Memory &mem);
        virtual void draw() const = 0;
    private:
        Memory &mem;
        friend class TerminalRenderer;
        friend class SdlRenderer;

};

class TerminalRenderer : public Renderer
{
    public:
        void draw() const;
        TerminalRenderer(Memory &mem);
};

class SdlRenderer : public Renderer
{
    public:
        void draw() const;
        SdlRenderer(Memory &mem, const int scale);
        ~SdlRenderer();
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};

