#ifndef CORE_IRENDER_H
#define CORE_IRENDER_H

#include <string>

#include "core/String.h"

namespace core
{
    class IRender
    {
    public:
        virtual ~IRender(){}
        
        virtual void showTask(const String &str) = 0;
        virtual void showInvalid(const String &str) = 0;
        virtual void showAnswer(const String &str) = 0;
        // TODO: show valid
    };
}

#endif
