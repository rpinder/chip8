#include <string>
#include <array>

class Memory
{
    private:
        std::array<unsigned char, 4096> data;
        std::array<unsigned char, 64 * 32> display;
    public:
        auto load(std::string title) -> void;
};
