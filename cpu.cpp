#include "cpu.hpp"

#include <iostream>
#include <unistd.h>

Cpu::Cpu(Memory& mem, std::array<bool, 16>& key)
    : mem(mem), key(key), rand(util::Random(0, 255))
{
    pc = 0x200;
    I = 0;
    sp = 0;

    std::fill(registers.begin(), registers.end(), 0);
    std::fill(stack.begin(), stack.end(), 0);

    timer_counter = 0;
    delay_timer = 0;
    sound_timer = 0;

}

auto Cpu::cycle() -> void
{
    drawFlag = false;
    std::cout << "[";
    for (const auto& r : registers) {
        std::cout << std:: hex << (int) r << ", ";
    }
    std::cout << "]" << std::endl;

    unsigned short opcode = fetch();
    execute(opcode);
    if (timer_counter % 8 == 0) {
        if (sound_timer > 0)
            sound_timer--;
        if (delay_timer > 0)
            delay_timer--;
    }

    timer_counter++;
}

auto Cpu::fetch() -> unsigned short
{
    unsigned short opcode = mem.data[pc] << 8 | mem.data[pc + 1];
    return opcode;
}

auto Cpu::execute(unsigned short opcode) -> void
{
    std::cout << std::hex << opcode << " | " << std::hex << pc << " " << std::hex << I << " " << std::hex << sp << " | ";
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    std::cout << "00E0 CLS" << std::endl;
                    std::fill(mem.display.begin(), mem.display.end(), 0);
                    drawFlag = true;
                    pc += 2;
                    break;
                case 0x000E:
                    std::cout << "00EE RET" << std::endl;
                    sp--;
                    pc = stack[sp];
                    pc += 2;
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
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            std::cout << "3xkk - SE Vx, byte" << std::endl;
            if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        case 0x4000:
            std::cout << "4xkk - SNE Vx, bye" << std::endl;
            if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        case 0x5000:
            std::cout << "5xy0 - SE Vx, Vy" << std::endl;
            if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
            break;
        case 0x6000:
            std::cout << "6xkk - LD Vx, byte" << std::endl;
            registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000:
            std::cout << "7xkk - ADD Vx, byte" << std::endl;
            registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    std::cout << "8xy0 - LD VX, Vy" << std::endl;
                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0001:
                    std::cout << "8xy1 - OR Vx, Vy" << std::endl;
                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] | registers[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0002:
                    std::cout << "8xy2 - AND Vx, Vy" << std::endl;
                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] & registers[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0003:
                    std::cout << "8xy3 - XOR Vx, Vy" << std::endl;
                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] ^ registers[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0004:
                    std::cout << "8xy4 - ADD Vx, Vy" << std::endl;
                    registers[(opcode & 0x0F00) >> 8] += registers[(opcode & 0x00F0) >> 4];
                    if(registers[(opcode & 0x00F0) >> 4] > (0xFF - registers[(opcode & 0x0F00) >> 8]))
                        registers[0xF] = 1; //carry
                    else
                        registers[0xF] = 0;
                    pc += 2;
                    break;
                case 0x0005:
                    std::cout << "8xy5 - SUB Vx, Vy" << std::endl;
                    if (registers[(opcode & 0x0F00) >> 8] >= registers[(opcode & 0x00F0) >> 4])
                        registers[15] = 1;
                    else
                        registers[15] = 0;

                    registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                case 0x0006:
                    std::cout << "8xy6 - SHR Vx {, Vy}" << std::endl;
                    registers[15] = registers[(opcode & 0x0F00) >> 8] & 1;
                    registers[(opcode & 0x0F00) >> 8] /= 2;
                    pc += 2;
                    break;
                case 0x0007:
                    std::cout << "8xy7 - SUBN Vx, Vy" << std::endl;
                    if (registers[(opcode & 0x0F00) >> 8] > registers[(opcode & 0x00F0) >> 4])
                        registers[15] = 0;
                    else
                        registers[15] = 1;

                    registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x000E:
                    std::cout << "8xyE - SHL Vx {, Vy}" << std::endl;
                    registers[15] = registers[(opcode & 0x0F00) >> 8] >> 7;
                    registers[(opcode & 0x0F00) >> 8] *= 2;
                    pc += 2;
                    break;
                default:
                    std::cout << "unknown opcode" << std::endl;
                    break;
            }
            break;
        case 0x9000:
            std::cout << "9xy0 - SNE Vx, Vy" << std::endl;
            if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
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
            pc = (opcode & 0x0FFF) + registers[0];
            break;
        case 0xC000:
            std::cout << "Cxkk - RND Vx, byte" << std::endl;
            registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & ((unsigned char) rand());
            pc += 2;
            break;
        case 0xD000:
            std::cout << "Dxyn - DRW Vx, Vy, nibble" << std::endl;
            {
                unsigned short x = registers[(opcode & 0x0F00) >> 8];
                unsigned short y = registers[(opcode & 0x00F0) >> 4];
                unsigned short height = opcode & 0x000F;
                unsigned short pixel;

                registers[0xF] = 0;
                for (int yline = 0; yline < height; yline++)
                {
                    pixel = mem.data[I + yline];
                    for(int xline = 0; xline < 8; xline++)
                    {
                        if((pixel & (0x80 >> xline)) != 0)
                        {
                            if(mem.display[(x + xline + ((y + yline) * 64))] == 1)
                                registers[0xF] = 1;                                 
                            mem.display[x + xline + ((y + yline) * 64)] = !mem.display[x + xline + ((y + yline) * 64)] != false;
                        }
                    }
                }

                drawFlag = true;
                pc += 2;
            }
            break;
            break;
        case 0xE000:
            switch (opcode & 0x00F0) {
                case 0x0090:
                    std::cout << "Ex9E - SKP Vx" << std::endl;
                    if (key[registers[(opcode & 0x0F00) >> 8]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                case 0x00A0:
                    std::cout << "ExA1 - SKNP Vx" << std::endl;
                    if (!key[registers[(opcode & 0x0F00) >> 8]])
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
                            registers[(opcode & 0x0F00) >> 8] = delay_timer;
                            pc += 2;
                            break;
                        case 0x000A:
                            std::cout << "Fx0A - LD Vx, K" << std::endl;
                            {
                                bool key_pressed = false;
                                for (int i = 0; i < 16; ++i) {
                                    if (key[i] != 0) {
                                        registers[(opcode & 0x0F00) >> 8] = i;
                                        key_pressed = true;
                                    }
                                }

                                if (!key_pressed) return;
                                pc += 2;
                            }
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
                            delay_timer = registers[(opcode & 0x0F00) >> 8];
                            pc += 2;
                            break;
                        case 0x0008:
                            std::cout << "Fx18 - LD ST, Vx" << std::endl;
                            sound_timer = registers[(opcode & 0x0F00) >> 8];
                            pc += 2;
                            break;
                        case 0x000E:
                            std::cout << "Fx1E - ADD I, Vx" << std::endl;
                            if (I + registers[(opcode & 0x0F00) >> 8] > 0xFFF)
                                registers[15] = 1;
                            else
                                registers[15] = 0;
                            I += registers[(opcode & 0x0F00) >> 8];
                            pc += 2;
                            break;
                        default:
                            std::cout << "unknown opcode" << std::endl;
                            break;
                    }
                    break;
                case 0x0020:
                    std::cout << "Fx29 - LD F, Vx" << std::endl;
                    I = registers[(opcode & 0x0F00) >> 8] * 5;
                    pc += 2;
                    break;
                case 0x0030:
                    std::cout << "Fx33 - LD B, Vx" << std::endl;
                    mem.data[I]    = registers[(opcode & 0x0F00) >> 8] / 100;
                    mem.data[I + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) % 10;
                    mem.data[I + 2] = registers[(opcode & 0x0F00) >> 8] % 10;
                    pc += 2;
                    break;
                case 0x0050:
                    std::cout << "Fx55 - LD [I], Vx" << std::endl;
                    for (int i = 0; i < ((opcode & 0x0F00) >> 8); i++) {
                       mem.data[I + i] = registers[i]; 
                    }
                    pc += 2;
                    break;
                case 0x0060:
                    std::cout << "Fx65 - LD Vx, [I]" << std::endl;
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
                        registers[i] = mem.data[I + i];
                    }
                    I += ((opcode & 0x0F00) >> 8) + 1;
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
