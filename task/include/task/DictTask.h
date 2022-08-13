#ifndef TASK_DICTTASK_H
#define TASK_DICTTASK_H

#include <string>
#include <unordered_set>

#include "task/BaseTask.h"
#include "task/StringCollection.h"

namespace task
{
    class DictTask: public BaseTask
    {
    public:
        DictTask(const StringCollection &keys, const StringCollection &values,
            int seed);

        bool validateBase(const core::String &result) override;
        core::String answer() const override;
        void describe(core::IRender &render) const override;
        void hint(core::IRender &render, std::size_t level) const override;

    private:
        using StringSet = std::unordered_set<core::String>;

    private:
        StringCollection keys;
        StringCollection values;
        StringSet answers;
        int seed;
    };
}

#endif
