#ifndef TASK_DICTTASK_H
#define TASK_DICTTASK_H

#include <string>

#include "core/ITask.h"

namespace task
{
    class DictTask: public core::ITask
    {
    public:
        DictTask(const std::string &key, const std::string &value);
        
        virtual bool validate(const ValueType &result);
        virtual void describe(core::IRender &render);

    private:
        std::string key;
        std::string value;
    };
}

#endif
