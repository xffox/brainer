#ifndef BASE_RANDOMIZER_H
#define BASE_RANDOMIZER_H

#include <type_traits>
#include <random>
#include <limits>

namespace base
{
    class Randomizer
    {
        using Rand = std::minstd_rand;

    public:
        using Seed = Rand::result_type;

    public:
        Randomizer();
        explicit Randomizer(Seed seed);
        Randomizer(Randomizer &&that) noexcept;

        [[nodiscard]]
        Randomizer copy() const;
        Randomizer diverge();

        // undefined behaviour if min > max
        template<typename T>
        std::enable_if_t<std::is_integral_v<T>, T> uniformInteger(
            T min = std::numeric_limits<T>::min(),
            T max = std::numeric_limits<T>::max())
        {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(rand);
        }

        template<typename T>
        std::enable_if_t<std::is_floating_point_v<T>, T> uniformReal(
            T min = std::numeric_limits<T>::lower(),
            T max = std::numeric_limits<T>::max())
        {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(rand);
        }

    private:
        explicit Randomizer(const Rand &rand);

    private:
        static std::random_device rnd_dev;

        Rand rand;
    };
}

#endif
