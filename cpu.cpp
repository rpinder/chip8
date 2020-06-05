#include "cpu.hpp"
#include "util.hpp"

#include <iostream>

Cpu::Cpu(Memory& mem, Renderer& renderer)
    : mem(mem), renderer(renderer)
{
    pc = 0x200;
    I = 0;
    sp = -1;

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
    std::cout << std::hex << opcode;
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    std::cout << "00E0 CLS" << std::endl;
                    std::fill(mem.display.begin(), mem.display.end(), 0);
                    pc += 2;
                    break;
                case 0x000E:
                    std::cout << "00EE RET" << std::endl;
                    pc = stack[sp];
                    sp--;
                    break;
                default:
                    std::cout << "unknown opcode" << std::endl;
                    break;
            }
            break;
        case 0x1000:
            std::cout << "1nnn - JP addr" << std::endl;
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            std::cout << "2nnn - CALL addr" << std::endl;
            sp++;
            stack[sp] = pc;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            std::cout << "3xkk - SE Vx, byte" << std::endl;
            if (registers[opcode & 0x0F00] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        case 0x4000:
            std::cout << "4xkk - SNE Vx, bye" << std::endl;
            if (registers[opcode & 0x0F00] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        case 0x5000:
            std::cout << "5xy0 - SE Vx, Vy" << std::endl;
            if (registers[opcode & 0x0F00] == registers[opcode & 0x00F0])
                pc += 4;
            else
                pc += 2;
            break;
        case 0x6000:
            std::cout << "6xkk - LD Vx, byte" << std::endl;
            registers[opcode & 0x0F00] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000:
            std::cout << "7xkk - ADD Vx, byte" << std::endl;
            registers[opcode & 0x0F00] += opcode & 0x00FF;
            pc += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    std::cout << "8xy0 - LD VX, Vy" << std::endl;
                    registers[opcode & 0x0F00] = registers[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0001:
                    std::cout << "8xy1 - OR Vx, Vy" << std::endl;
                    registers[opcode & 0x0F00] = registers[opcode & 0x0F00] | registers[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0002:
                    std::cout << "8xy2 - AND Vx, Vy" << std::endl;
                    registers[opcode & 0x0F00] = registers[opcode & 0x0F00] & registers[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0003:
                    std::cout << "8xy3 - XOR Vx, Vy" << std::endl;
                    registers[opcode & 0x0F00] = registers[opcode & 0x0F00] ^ registers[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0004:
                    std::cout << "8xy4 - ADD Vx, Vy" << std::endl;
                    registers[opcode & 0x0F00] += registers[opcode & 0x00F0];
                    if (registers[opcode & 0x0F00] > 255) {
                        registers[15] = 1;
                        registers[opcode & 0x0F00] -= 255;
                    } else {
                        registers[15] = 0;

                    }
                    pc += 2;
                    break;
                case 0x0005:
                    std::cout << "8xy5 - SUB Vx, Vy" << std::endl;
                    if (registers[opcode & 0x0F00] > registers[opcode & 0x00F0])
                        registers[15] = 1;
                    else
                        registers[15] = 0;

                    registers[opcode & 0x0F00] -= registers[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0006:
                    std::cout << "8xy6 - SHR Vx {, Vy}" << std::endl;
                    if (registers[opcode & 0x0F00] & 1)
                        registers[15] = 1;
                    else
                        registers[15] = 0;

                    registers[opcode & 0x0F00] /= 2;
                    pc += 2;
                    break;
                case 0x0007:
                    std::cout << "8xy7 - SUBN Vx, Vy" << std::endl;
                    if (registers[opcode & 0x0F00] > registers[opcode & 0x00F0])
                        registers[15] = 1;
                    else
                        registers[15] = 0;

                    registers[opcode & 0x0F00] = registers[opcode & 0x00F0] - registers[opcode & 0x0F00];
                    pc += 2;
                    break;
                case 0x000E:
                    std::cout << "8xyE - SHL Vx {, Vy}" << std::endl;
                    if (registers[opcode & 0x0F00] & (1 << 7))
                        registers[15] = 1;
                    else
                        registers[15] = 0;

                    registers[opcode & 0x0F00] *= 2;
                    pc += 2;
                    break;
                default:
                    std::cout << "unknown opcode" << std::endl;
                    break;
            }
            break;
        case 0x9000:
            std::cout << "9xy0 - SNE Vx, Vy" << std::endl;
            if (registers[opcode & 0x0F00] != registers[opcode & 0x00F0])
                pc += 4;
            else
                pc += 2;
            break;
        case 0xA000:
            std::cout << "Annn - LD I, addr" << std::endl;
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000:
            std::cout << "Bnnn - JP V0, addr" << std::endl;
            pc = opcode & 0x0FFF + registers[0];
            break;
        case 0xC000:
            std::cout << "Cxkk - RND Vx, byte" << std::endl;
            registers[opcode & 0x0F00] = (opcode & 0x00FF) & ((unsigned char) util::random_int(0, 255));
            pc += 2;
            break;
        case 0xD000:
            std::cout << "Dxyn - DRW Vx, Vy, nibble" << std::endl;
            // TODO
            break;
        case 0xE000:
            switch (opcode & 0x00F0) {
                case 0x0090:
                    std::cout << "Ex9E - SKP Vx" << std::endl;
                    if (key[registers[opcode & 0x0F00]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                case 0x00A0:
                    std::cout << "ExA1 - SKNP Vx" << std::endl;
                    if (!key[registers[opcode & 0x0F00]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                default:
                    std::cout << "unknown opcode" << std::endl;
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00F0) {
                case 0x0000:
                    switch (opcode & 0x000F) {
                        case 0x0007:
                            std::cout << "Fx07 - LD Vx, DT" << std::endl;
                            registers[opcode & 0x0F00] = delay_timer;
                            pc += 2;
                            break;
                        case 0x000A:
                            std::cout << "Fx0A - LD Vx, K" << std::endl;
                            // TODO
                            break;
                        default:
                            std::cout << "unknown opcode" << std::endl;
                            break;
                    }
                    break;
                case 0x0010:
                    switch (opcode & 0x000F) {
                        case 0x0005:
                            std::cout << "Fx15 - LD DT, Vx" << std::endl;
                            delay_timer = registers[opcode & 0x0F00];
                            pc += 2;
                            break;
                        case 0x0008:
                            std::cout << "Fx18 - LD ST, Vx" << std::endl;
                            sound_timer = registers[opcode & 0x0F00];
                            pc += 2;
                            break;
                        case 0x000E:
                            std::cout << "Fx1E - ADD I, Vx" << std::endl;
                            I += registers[opcode & 0x0F00];
                            pc += 2;
                            break;
                        default:
                            std::cout << "unknown opcode" << std::endl;
                            break;
                    }
                    break;
                case 0x0020:
                    std::cout << "Fx29 - LD F, Vx" << std::endl;
                    // TODO
                    break;
                case 0x0030:
                    std::cout << "Fx33 - LD B, Vx" << std::endl;
                    // TODO 
                    break;
                case 0x0050:
                    std::cout << "Fx55 - LD [I], Vx" << std::endl;
                    for (int i = 0; i < 15; i++) {
                       mem.data[I + i] = registers[i]; 
                    }
                    pc += 2;
                    break;
                case 0x0060:
                    std::cout << "Fx65 - LD Vx, [I]" << std::endl;
                    for (int i = 0; i < 15; i++) {
                        registers[i] = mem.data[I + i];
                    }
                    pc += 2;
                   break;
                default:
                    std::cout << "unknown opcode" << std::endl;
                    break;
            }
            break;
        default:
            std::cout << "unknown opcode" << std::endl;
            break;
    }
}

auto Cpu::drawing() -> bool
{
    return drawFlag;
}
