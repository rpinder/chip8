#include "util.hpp"

#include <random>

auto util::random_int(int start, int end) -> int
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
    return dist(rng);
}
