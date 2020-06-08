#include "memory.hpp"
#include "util.hpp"

#include <array>

class Cpu
{
    private:
        std::array<unsigned char, 16> registers;
        std::array<unsigned short, 16> stack;
        unsigned short sp;
        unsigned short I;
        unsigned short pc;
        int timer_counter;
        unsigned char delay_timer;
        unsigned char sound_timer;


        bool drawFlag;

        auto fetch() -> unsigned short;
        auto execute(unsigned short opcode) -> void;

        Memory& mem;
        std::array<bool, 16>& key;

        util::Random rand;
    public:
        Cpu(Memory& mem, std::array<bool, 16>& key);
        auto cycle() -> void;
        auto drawing() -> bool;
};
