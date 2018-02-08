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
    constexpr std::size_t COLUMNS = 2;

    void process(std::wistream &stream)
    {
        csv::Csv<wchar_t> reader(stream, L',', L'"', L'\0');
        while(true)
        {
            auto row = reader.row();
            if(!row.second)
                break;
            if(row.first.size() != COLUMNS)
                throw std::runtime_error("invalid row format");
            auto preparedDescription =
                task::inner::wiktionary::prepareDescription(
                row.first[1], row.first[0]);
            std::cout<<'<'<<std::endl;
            std::wcout<<row.first[0]<<std::endl<<"--->"<<std::endl;
            std::wcout<<row.first[1]<<std::endl<<"===="<<std::endl;
            std::wcout<<preparedDescription<<std::endl;
            std::cout<<'>'<<std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr<<"usage: "<<argv[0]<<" <filename>"<<std::endl;
        return 1;
    }
    std::wifstream stream(argv[1]);
    stream.imbue(std::locale(std::locale("")));
    if(!stream.is_open())
    {
        std::cerr<<"can't read file: '"<<argv[1]<<'\''<<std::endl;
        return 1;
    }
    process(stream);
    return 0;
}
