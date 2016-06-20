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
#include "task/MultiLetterTaskGenerator.h"
#include "task/FactTaskGenerator.h"
#include "task/StringCollection.h"
#include "task/FactDescriptionTaskGenerator.h"
#include "task/tagaini.h"
#include "task/StringSet.h"
#include "fact/FileFact.h"
#include "csv/csv.h"
#include "base/strutil.h"

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

        MultiLetterTaskGenerator::LetterCol readMultiLetterTasksFromConfig(
            const base::IConfig::ValuesCollection &config)
        {
            MultiLetterTaskGenerator::LetterCol res;
            std::transform(config.begin(), config.end(),
                std::back_inserter(res), [](
                    const base::IConfig::ValuesCollection::value_type &p) {
                    return std::make_pair(p.second, p.first);
                });
            return res;
        }

        DictTaskGenerator::TaskCollection readTagainiWithProns(
            const std::string &filename)
        {
            std::wifstream stream(filename);
            stream.imbue(std::locale(std::locale("")));
            if(!stream.is_open())
                throw std::runtime_error("can't read file");
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
                throw std::runtime_error("can't read file");
            const auto res = tagaini::readCollection(stream);
            DictTaskGenerator::TaskCollection tasks;
            transform(res.begin(), res.end(), std::back_inserter(tasks),
                [](const tagaini::TaskCollection::value_type &v) {
                return std::make_pair(StringCollection{std::get<0>(v)}, std::get<2>(v));
                });
            return tasks;
        }

        DictTaskGenerator::TaskCollection readTagainiPronouns(
            const std::string &filename)
        {
            std::wifstream stream(filename);
            stream.imbue(std::locale(std::locale("")));
            if(!stream.is_open())
                throw std::runtime_error("can't read file");
            const auto res = tagaini::readCollection(stream);
            DictTaskGenerator::TaskCollection tasks;
            transform(res.begin(), res.end(), std::back_inserter(tasks),
                [](const tagaini::TaskCollection::value_type &v) {
                    const auto &pronouns = std::get<1>(v);
                    const auto &translations = std::get<2>(v);
                    const core::String *key = nullptr;
                    if(!pronouns.empty())
                        key = &pronouns[0];
                    else
                        key = &std::get<0>(v);
                    assert(key);
                    std::wstringstream valueStream;
                    valueStream<<*key<<L" ["<<std::get<0>(v)
                    <<L"] ("
                    <<base::strutil::join(translations.begin(), translations.end(), core::String(L", "))
                    <<L')';
                return std::make_pair(StringCollection{*key},
                    StringCollection{valueStream.str()});
                });
            return tasks;
        }

        class PropertyPredicate: public FactTaskGenerator::Predicate
        {
        public:
            PropertyPredicate()
                :properties{
                    L"child astronomical body",
                    L"parent astronomical body"
                }
            {}

            virtual bool accept(const core::String &property) const override
            {
                return properties.find(property) != properties.end();
            }

        private:
            task::StringSet properties;
        };
    }

    TaskProvider::TaskProvider(const std::string &configFilename)
        :configFilename(configFilename), creators{
            std::make_pair("hex", [](const std::string&) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new HexByteTaskGenerator(rand()));
                }),
            std::make_pair("arithmetic", [](const std::string&) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new ArithmeticTaskGenerator(rand()));
                }),
            std::make_pair("config", [](const std::string &filename) {
                base::FileConfig fc(filename);
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(),
                        readTasksFromConfig(fc.read())));
                }),
            std::make_pair("tagaini", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(),
                        readTagaini(filename)));
                }),
            std::make_pair("tagaini_rev", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(),
                        readTagaini(filename), true));
                }),
            std::make_pair("tagaini_prons", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(),
                        readTagainiWithProns(filename)));
                }),
            std::make_pair("tagaini_prons_rev", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(),
                        readTagainiWithProns(filename), true));
                }),
            std::make_pair("multiletter", [](const std::string &filename) {
                base::FileConfig fc(filename);
                return std::unique_ptr<core::ITaskGenerator>(
                    new MultiLetterTaskGenerator(rand(),
                        readMultiLetterTasksFromConfig(fc.read())));
                }),
            std::make_pair("fact", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new FactTaskGenerator(rand(),
                        std::unique_ptr<fact::IFact>(
                            new fact::FileFact(filename)),
                        std::unique_ptr<FactTaskGenerator::Predicate>(
                            new PropertyPredicate())));
                }),
            std::make_pair("description", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new FactDescriptionTaskGenerator(
                        std::unique_ptr<fact::IFact>(
                            new fact::FileFact(filename)),
                        rand()));
                }),
            std::make_pair("tagaini_typing", [](const std::string &filename) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(rand(),
                        readTagainiPronouns(filename)));
                }),
        }
    {}

    TaskProvider::StringSet TaskProvider::getTasks() const
    {
        const auto tasksConfig = readTasksConfig(configFilename);
        StringSet result;
        std::transform(tasksConfig.begin(), tasksConfig.end(),
            std::inserter(result, result.begin()),
            [](const TaskConfigCollection::value_type &p) {
                return p.name;
            });
        return result;
    }

    std::unique_ptr<core::ITaskGenerator> TaskProvider::create(
        const std::string &name)
    {
        const auto tasksConfig = readTasksConfig(configFilename);
        for(const auto &task : tasksConfig)
        {
            if(task.name == name)
            {
                auto iter = creators.find(task.type);
                if(iter == creators.end())
                    throw std::runtime_error("invalid task type configured");
                return iter->second(task.filename);
            }
        }
        throw std::invalid_argument("invalid task name");
    }
}
