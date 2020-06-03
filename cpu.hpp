#include <array>
#include "memory.hpp"
#include "renderer.hpp"

class Cpu
{
    private:
        std::array<unsigned char, 16> registers;
        std::array<unsigned short, 16> stack;
        unsigned short sp;
        unsigned short I;
        unsigned short pc;
        unsigned char delay_timer;
        unsigned char sound_timer;
        
        bool drawFlag;

    public:
        Cpu(Memory& mem, Renderer& renderer);
        auto cycle() -> void;
        auto drawing() -> bool;
        auto setKeys() -> void;
};
