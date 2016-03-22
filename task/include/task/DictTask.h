#ifndef TASK_DICTTASK_H
#define TASK_DICTTASK_H

#include <string>

#include "core/ITask.h"

namespace task
{
    class DictTask: public core::ITask
    {
    public:
        DictTask(const core::String &key, const core::String &value);
        
        virtual bool validate(const core::String &result) const;
        virtual core::String answer() const;
        virtual void describe(core::IRender &render) const;

    private:
        core::String key;
        core::String value;
    };
}

#endif
