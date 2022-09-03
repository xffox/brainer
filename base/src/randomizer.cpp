#include "base/randomizer.hpp"

#include <limits>

namespace base
{
    Randomizer::Randomizer()
        :rand([](){
                return std::uniform_int_distribution<Seed>(
                    std::numeric_limits<Seed>::min(),
                    std::numeric_limits<Seed>::max())(rnd_dev);
            }())
    {}

    Randomizer::Randomizer(Seed seed)
        :rand(seed)
    {}

    Randomizer::Randomizer(Randomizer &&that) noexcept
        :rand(that.rand)
    {}

    Randomizer::Randomizer(const Rand &rand)
        :rand(rand)
    {}

    Randomizer Randomizer::copy() const
    {
        return Randomizer(rand);
    }

    Randomizer Randomizer::diverge()
    {
        return Randomizer(uniformInteger<Seed>());
    }

    std::random_device Randomizer::rnd_dev{};
}
