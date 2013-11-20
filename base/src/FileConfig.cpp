#include "base/FileConfig.h"

#include <string>
#include <fstream>
#include <exception>

namespace base
{
    FileConfig::FileConfig(const std::string &filename)
        :filename(filename)
    {
    }

    IConfig::ValuesMap FileConfig::read()
    {
        std::ifstream stream(filename.c_str());
        if(!stream.is_open())
            throw std::exception();
        return readStream(stream);
    }
}
