#include "memory.hpp"

#include <fstream>
#include <iostream>

unsigned char chip8_fontset[80] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Memory::Memory() 
{
    std::fill(data.begin(), data.end(), 0);
    std::fill(display.begin(), display.end(), 0);

    for (int i = 0; i < 80; ++i)
        data[i] = chip8_fontset[i];

}

auto Memory::load(std::string title) -> void
{
    std::ifstream myfile(title, std::ios::binary);
    if (myfile.is_open()) {
        
        std::istreambuf_iterator<char> it(myfile);
        std::istreambuf_iterator<char> eof;
        for (int i = 512; it != eof; ++i, ++it) {
            data[i] = *it; 
        }
        myfile.close();
    } else {
        std::cerr << "Unable to open rom" << std::endl;
        exit(1);
    }
    
}
