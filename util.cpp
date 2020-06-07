#include "util.hpp"

util::Random::Random(int start, int end)
    : rng{std::random_device{}()}, dist{start, end}
{
}

auto util::Random::operator()() -> int
{
    return dist(rng);
}
