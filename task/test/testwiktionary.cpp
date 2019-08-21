#include <istream>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <stdexcept>
#include <locale>

#include "task/inner/wiktionary.h"
#include "csv/csv.h"

namespace
{
    template<typename CharT>
    std::size_t bench(std::basic_istream<CharT> &stream)
    {
        csv::Csv<CharT> reader(stream, ',', '"', '\0');
        std::size_t len = 0;
        while(true)
        {
            auto row = reader.row();
            if(!row.second)
                break;
            len += row.first.size();
        }
        return len;
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr<<"usage: "<<argv[0]<<" <filename>"<<std::endl;
        return 1;
    }
    {
        std::ifstream stream(argv[1]);
        stream.imbue(std::locale(std::locale("")));
        if(!stream.is_open())
        {
            std::cerr<<"can't read file: '"<<argv[1]<<'\''<<std::endl;
            return 1;
        }
        std::cout<<bench(stream)<<std::endl;
    }
    return 0;
}
