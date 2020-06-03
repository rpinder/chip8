#include <string>
#include <array>

class Memory
{
    public:
        auto load(std::string title) -> void;
        std::array<unsigned char, 4096> data;
        std::array<unsigned char, 64 * 32> display;
};
