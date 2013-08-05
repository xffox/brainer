#ifndef CORE_ITASK_H
#define CORE_ITASK_H

#include <string>

namespace core
{
    class IRender;

    class ITask
    {
    public:
        virtual ~ITask(){}
        
        virtual bool validate(const std::string &result) = 0;
        virtual void describe(IRender &render) = 0;
    };
}

#endif
