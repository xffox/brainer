#include "task/TaskProvider.h"

#include <exception>
#include <cstdlib>
#include <fstream>

#include "base/FileConfig.h"
#include "core/ITaskGenerator.h"
#include "task/HexByteTaskGenerator.h"
#include "task/MultiplicationTaskGenerator.h"
#include "task/DictTaskGenerator.h"
#include "task/tagaini.h"

namespace task
{
    TaskProvider::TaskProvider()
        :tasks()
    {
        tasks.insert("hex");
        tasks.insert("multiplication");
        tasks.insert("japanese");
        tasks.insert("tagaini");
    }

    TaskProvider::StringSet TaskProvider::getTasks() const
    {
        return tasks;
    }

    std::auto_ptr<core::ITaskGenerator> TaskProvider::create(
        const std::string &name)
    {
        if(name == "hex")
        {
            return std::auto_ptr<core::ITaskGenerator>(
                new HexByteTaskGenerator(rand()));
        }
        else if(name == "multiplication")
        {
            return std::auto_ptr<core::ITaskGenerator>(
                new MultiplicationTaskGenerator(rand()));
        }
        else if(name == "japanese")
        {
            try
            {
                base::FileConfig fc("japanese.txt");
                return std::auto_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(), fc.read()));
            }
            catch(const std::exception&)
            {
            }
        }
        else if(name == "tagaini")
        {
            try
            {
                std::wifstream stream("/home/fox/temp/export.tsv");
                stream.imbue(std::locale(std::locale("")));
                if(!stream.is_open())
                    throw std::exception();
                return std::auto_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(), tagaini::readCollection(stream)));
            }
            catch(const std::exception&)
            {
            }
        }
        throw std::exception();
    }
}
