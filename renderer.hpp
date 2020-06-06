#include "memory.hpp"

class Renderer
{
    friend class TerminalRenderer;
    friend class SdlRenderer;
    public:
        Renderer(Memory &mem);
        virtual void draw() const = 0;
    private:
        Memory &mem;
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
        SdlRenderer(Memory &mem);
};

