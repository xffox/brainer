#ifndef TASK_MULTIPLICATIONTASK_H
#define TASK_MULTIPLICATIONTASK_H

#include "core/ITask.h"

namespace task
{
    class MultiplicationTask: public core::ITask
    {
        friend class MultiplicationTaskGenerator;
    public:
        virtual bool validate(const std::string &result);
        virtual void describe(core::IRender &render);

    private:
        MultiplicationTask(int a, int b)
            :a(a), b(b), product(a*b)
        {}
        
    private:
        int a;
        int b;
        int product;
    };
}

#endif
