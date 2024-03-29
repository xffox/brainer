#include "base/FileConfig.h"

#include <string>
#include <fstream>
#include <stdexcept>
#include <locale>

namespace base
{
    FileConfig::FileConfig(const std::string &filename)
        :filename(filename)
    {
    }

    IConfig::ValuesCollection FileConfig::read()
    {
        std::wifstream stream(filename.c_str());
        stream.imbue(std::locale(std::locale("")));
        if(!stream.is_open())
            throw std::runtime_error("can't read file");
        return readStream(stream);
    }
}
