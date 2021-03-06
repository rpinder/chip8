#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <array>

class Memory
{
    public:
        Memory();
        auto load(std::string title) -> void;
        std::array<unsigned char, 4096> data;
        std::array<bool, 64 * 32> display;
};

#endif
