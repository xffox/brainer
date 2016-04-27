#ifndef TASK_DICTTASK_H
#define TASK_DICTTASK_H

#include <string>
#include <unordered_set>

#include "core/ITask.h"
#include "task/StringCollection.h"

namespace task
{
    class DictTask: public core::ITask
    {
    public:
        DictTask(const StringCollection &keys, const StringCollection &values);
        
        virtual bool validate(const core::String &result) const;
        virtual core::String answer() const;
        virtual void describe(core::IRender &render) const;

    private:
        using StringSet = std::unordered_set<core::String>;

    private:
        StringCollection keys;
        StringCollection values;
        StringSet answers;
    };
}

#endif
