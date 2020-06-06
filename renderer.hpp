#include "memory.hpp"

class Renderer
{
    private:
        Memory& mem;
    public:
        Renderer(Memory& mem);
        auto draw() -> void;
};
