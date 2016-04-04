#include "task/TaskProvider.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <utility>

#include "base/FileConfig.h"
#include "core/ITaskGenerator.h"
#include "task/HexByteTaskGenerator.h"
#include "task/ArithmeticTaskGenerator.h"
#include "task/DictTaskGenerator.h"
#include "task/StringCollection.h"
#include "task/tagaini.h"
#include "csv/csv.h"

namespace task
{
    namespace
    {
        DictTaskGenerator::TaskCollection fromConfig(
            const base::IConfig::ValuesCollection &config)
        {
            DictTaskGenerator::TaskCollection res;
            std::transform(config.begin(), config.end(),
                std::back_inserter(res), [](
                    const base::IConfig::ValuesCollection::value_type &p) {
                    return std::make_pair(p.second, StringCollection{p.first});
                });
            return res;
        }
    }

    TaskProvider::TaskProvider()
        :tasks{
            std::make_pair("hex", []() {
                    return std::auto_ptr<core::ITaskGenerator>(
                        new HexByteTaskGenerator(rand()));
                }),
            std::make_pair("arithmetic", []() {
                    return std::auto_ptr<core::ITaskGenerator>(
                        new ArithmeticTaskGenerator(rand()));
                }),
            std::make_pair("hiragana", []() {
                    base::FileConfig fc("hiragana.txt");
                    return std::auto_ptr<core::ITaskGenerator>(
                        new DictTaskGenerator(rand(), fromConfig(fc.read())));
                }),
            std::make_pair("katakana", []() {
                    base::FileConfig fc("katakana.txt");
                    return std::auto_ptr<core::ITaskGenerator>(
                        new DictTaskGenerator(rand(), fromConfig(fc.read())));
                }),
            std::make_pair("tagaini", []() {
                    std::wifstream stream("numbers.tsv");
                    stream.imbue(std::locale(std::locale("")));
                    if(!stream.is_open())
                        throw std::exception();
                    return std::auto_ptr<core::ITaskGenerator>(
                        new DictTaskGenerator(rand(),
                            tagaini::readCollection(stream)));
                }),
            std::make_pair("revtagaini", []() {
                    std::wifstream stream("numbers.tsv");
                    stream.imbue(std::locale(std::locale("")));
                    if(!stream.is_open())
                        throw std::exception();
                    return std::auto_ptr<core::ITaskGenerator>(
                        new DictTaskGenerator(rand(),
                            tagaini::readCollection(stream), true));
                })
        }
    {}

    TaskProvider::StringSet TaskProvider::getTasks() const
    {
        StringSet result;
        std::transform(tasks.begin(), tasks.end(),
            std::inserter(result, result.begin()),
            [](const TaskMap::value_type &p) {
                return p.first;
            });
        return result;
    }

    std::auto_ptr<core::ITaskGenerator> TaskProvider::create(
        const std::string &name)
    {
        auto iter = tasks.find(name);
        if(iter != tasks.end())
            return iter->second();
        else
            throw std::invalid_argument("invalid task name");
    }
}
