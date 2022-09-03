#ifndef BASE_FILECONFIG_H
#define BASE_FILECONFIG_H

#include <string>

#include "base/StreamConfig.h"

namespace base
{
    class FileConfig: public StreamConfig
    {
    public:
        FileConfig(const std::string &filename);

        ValuesCollection read() override;

    private:
        std::string filename;
    };
}

#endif
