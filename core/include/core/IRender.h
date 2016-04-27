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
        
        virtual void addText(const String &str) = 0;
    };
}

#endif
