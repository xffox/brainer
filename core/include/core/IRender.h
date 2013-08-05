#ifndef CORE_IRENDER_H
#define CORE_IRENDER_H

#include <string>

namespace core
{
    class IRender
    {
    public:
        virtual ~IRender(){}
        
        virtual void addText(const std::string &str) = 0;
    };
}

#endif
