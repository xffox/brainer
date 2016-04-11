#include "task/TaskProvider.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <vector>
#include <cstddef>

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
        struct TaskConfig
        {
            std::string name;
            std::string type;
            std::string filename;
        };

        using TaskConfigCollection = std::vector<TaskConfig>;

        TaskConfigCollection readTasksConfig(const std::string &filename)
        {
            const std::size_t COLUMNS = 3;
            std::ifstream stream(filename);
            if(!stream.is_open())
                throw std::runtime_error("can't read tasks config file");
            csv::Csv<char> reader(stream);
            TaskConfigCollection tasksConfig;
            while(true)
            {
                const auto row = reader.row();
                if(!row.second)
                    break;
                if(row.first.size() != COLUMNS)
                    throw new std::runtime_error(
                        "invalid number of columns in tasks config file");
                tasksConfig.push_back(TaskConfig{row.first[0], row.first[1],
                    row.first[2]});
            }
            return tasksConfig;
        }

        DictTaskGenerator::TaskCollection readTasksFromConfig(
            const base::IConfig::ValuesCollection &config)
        {
            DictTaskGenerator::TaskCollection res;
            std::transform(config.begin(), config.end(),
                std::back_inserter(res), [](
                    const base::IConfig::ValuesCollection::value_type &p) {
                    return std::make_pair(StringCollection{p.second}, StringCollection{p.first});
                });
            return res;
        }

        DictTaskGenerator::TaskCollection readTagainiWithProns(
            const std::string &filename)
        {
            std::wifstream stream(filename);
            stream.imbue(std::locale(std::locale("")));
            if(!stream.is_open())
                throw std::exception();
            const auto res = tagaini::readCollection(stream);
            DictTaskGenerator::TaskCollection tasks;
            transform(res.begin(), res.end(), std::back_inserter(tasks),
                [](const tagaini::TaskCollection::value_type &v) {
                auto keys = std::get<1>(v);
                keys.insert(keys.begin(), std::get<0>(v));
                return std::make_pair(keys, std::get<2>(v));
                });
            return tasks;
        }

        DictTaskGenerator::TaskCollection readTagaini(
            const std::string &filename)
        {
            std::wifstream stream(filename);
            stream.imbue(std::locale(std::locale("")));
            if(!stream.is_open())
                throw std::exception();
            const auto res = tagaini::readCollection(stream);
            DictTaskGenerator::TaskCollection tasks;
            transform(res.begin(), res.end(), std::back_inserter(tasks),
                [](const tagaini::TaskCollection::value_type &v) {
                return std::make_pair(StringCollection{std::get<0>(v)}, std::get<2>(v));
                });
            return tasks;
        }
    }

    TaskProvider::TaskProvider(const std::string &configFilename)
        :tasks()
    {
        const auto tasksConfig = readTasksConfig(configFilename);
        for(const auto &taskConfig : tasksConfig)
        {
            if(taskConfig.type == "hex")
            {
                tasks.insert(std::make_pair(
                        taskConfig.name, []() {
                        return std::unique_ptr<core::ITaskGenerator>(
                            new HexByteTaskGenerator(rand()));
                        }));
            }
            if(taskConfig.type == "arithmetic")
            {
                tasks.insert(std::make_pair(
                        taskConfig.name, []() {
                        return std::unique_ptr<core::ITaskGenerator>(
                            new ArithmeticTaskGenerator(rand()));
                        }));
            }
            else if(taskConfig.type == "config")
            {
                const auto filename = taskConfig.filename;
                tasks.insert(std::make_pair(
                        taskConfig.name, [filename]() {
                        base::FileConfig fc(filename);
                        return std::unique_ptr<core::ITaskGenerator>(
                            new DictTaskGenerator(rand(),
                                readTasksFromConfig(fc.read())));
                        }));
            }
            else if(taskConfig.type == "tagaini")
            {
                const auto filename = taskConfig.filename;
                tasks.insert(std::make_pair(
                        taskConfig.name, [filename]() {
                        return std::unique_ptr<core::ITaskGenerator>(
                            new DictTaskGenerator(rand(),
                                readTagaini(filename)));
                        }));
            }
            else if(taskConfig.type == "tagaini_rev")
            {
                const auto filename = taskConfig.filename;
                tasks.insert(std::make_pair(
                        taskConfig.name, [filename]() {
                        return std::unique_ptr<core::ITaskGenerator>(
                            new DictTaskGenerator(rand(),
                                readTagaini(filename), true));
                        }));
            }
            else if(taskConfig.type == "tagaini_prons")
            {
                const auto filename = taskConfig.filename;
                tasks.insert(std::make_pair(
                        taskConfig.name, [filename]() {
                        return std::unique_ptr<core::ITaskGenerator>(
                            new DictTaskGenerator(rand(),
                                readTagainiWithProns(filename)));
                        }));
            }
            else if(taskConfig.type == "tagaini_prons_rev")
            {
                const auto filename = taskConfig.filename;
                tasks.insert(std::make_pair(
                        taskConfig.name, [filename]() {
                        return std::unique_ptr<core::ITaskGenerator>(
                            new DictTaskGenerator(rand(),
                                readTagainiWithProns(filename), true));
                        }));
            }
        }
    }

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

    std::unique_ptr<core::ITaskGenerator> TaskProvider::create(
        const std::string &name)
    {
        auto iter = tasks.find(name);
        if(iter != tasks.end())
            return iter->second();
        else
            throw std::invalid_argument("invalid task name");
    }
}
