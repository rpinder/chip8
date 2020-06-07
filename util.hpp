#include <random>

namespace util {
    class Random
    {
    private:
        std::mt19937 rng;
        std::uniform_int_distribution<int> dist;
    public:
        Random(int start, int end);
        int operator()();
    };
}
