#ifndef BASE_STREAMCONFIG_H
#define BASE_STREAMCONFIG_H

#include <istream>

#include "base/IConfig.h"

namespace base
{
    class StreamConfig: public IConfig
    {
    public:
        virtual ValuesMap readStream(std::istream &stream);
    };
}

#endif
