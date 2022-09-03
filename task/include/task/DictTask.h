#ifndef TASK_DICTTASK_H
#define TASK_DICTTASK_H

#include <string>
#include <unordered_set>

#include <base/randomizer.hpp>
#include "task/BaseTask.h"
#include "task/StringCollection.h"

namespace task
{
    class DictTask: public BaseTask
    {
    public:
        DictTask(base::Randomizer &&random,
            const StringCollection &keys, const StringCollection &values);

        bool validateBase(const core::String &result) override;
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) override;

    private:
        using StringSet = std::unordered_set<core::String>;

    private:
        StringCollection keys;
        StringCollection values;
        StringSet answers;
        base::Randomizer random;
    };
}

#endif
