#ifndef TASK_MARKOVCHAIN_H
#define TASK_MARKOVCHAIN_H

#include <vector>
#include <unordered_map>
#include <random>
#include <cstddef>

#include <base/randomizer.hpp>
#include "core/String.h"

namespace task
{
    class WordGen
    {
    public:
        using StringCol = std::vector<core::String>;

    public:
        WordGen(base::Randomizer &&random, const StringCol &words);
        core::String generate();

    private:
        using ProbMap = std::unordered_map<core::String::value_type, double>;
        using TransMap = std::unordered_map<core::String::value_type, ProbMap>;
        using LenMap = std::unordered_map<std::size_t, double>;

    private:
        static constexpr char EOW = '\0';

        TransMap transitions;
        ProbMap eowProbs;
        LenMap lengthes;
        base::Randomizer random;
    };
}

#endif
