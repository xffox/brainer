#ifndef CORE_ITASK_H
#define CORE_ITASK_H

#include <string>

#include "base/StringValue.h"

namespace core
{
    class IRender;

    class ITask
    {
    public:
        typedef base::StringValue ValueType;

    public:
        virtual ~ITask(){}
        
        virtual bool validate(const ValueType &result) = 0;
        virtual void describe(IRender &render) = 0;
    };
}

#endif
