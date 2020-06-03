#include "cpu.hpp"

Cpu::Cpu(Memory& mem, Renderer& renderer)
    : mem(mem), renderer(renderer)
{
    pc = 0x200;
    I = 0;
    sp = 0;

    delay_timer = 0;
    sound_timer = 0;
}

auto Cpu::cycle() -> void
{
    unsigned short opcode = fetch();
    execute(opcode);
}

auto Cpu::fetch() -> unsigned short
{
    unsigned short opcode = mem.data[pc] << 8 | mem.data[pc + 1];
    return opcode;
}

auto Cpu::execute(unsigned short opcode) -> void
{
    switch (opcode) {
    
    }
}

auto Cpu::drawing() -> bool
{
    return drawFlag;
}
