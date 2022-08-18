#include "task/TaskProvider.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <vector>
#include <cstddef>
#include <limits>

#include "base/FileConfig.h"
#include "core/ITaskGenerator.h"
#include "task/HexByteTaskGenerator.h"
#include "task/ArithmeticTaskGenerator.h"
#include "task/DictTaskGenerator.h"
#include "task/MultiLetterTaskGenerator.h"
#include "task/StringCollection.h"
#include "task/tagaini.h"
#include "task/MastermindTaskGenerator.h"
#include "task/StringSet.h"
#include "csv/csv.h"
#include "base/strutil.h"
#include "wordgen.h"
#include "task/inner/wiktionary.h"

namespace task
{
    namespace
    {
        struct TaskConfig
        {
            std::string name;
            std::string type;
            TaskProvider::StringCol filenames;
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
                std::stringstream ss(row.first[2]);
                csv::Csv<char> filenamesReader(ss, ';');
                TaskProvider::StringCol filenames;
                while(true)
                {
                    const auto filenamesRow = filenamesReader.row();
                    if(!filenamesRow.second)
                        break;
                    filenames.insert(filenames.end(),
                        filenamesRow.first.begin(), filenamesRow.first.end());
                }
                tasksConfig.push_back(TaskConfig{row.first[0], row.first[1],
                    filenames});
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

        template<class Generator>
        int genSeed(Generator &generator)
        {
            std::uniform_int_distribution<int> dis(
                std::numeric_limits<int>::min(),
                std::numeric_limits<int>::max());
            return dis(generator);
        }
    }

    TaskProvider::TaskProvider(const std::string &configFilename)
        :configFilename(configFilename),
        seed({std::random_device()()}), random(seed), creators{
            std::make_pair("hex", [this](const StringCol&) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new HexByteTaskGenerator(genSeed(random)));
                }),
            std::make_pair("arithmetic", [this](const StringCol&) {
                return std::unique_ptr<core::ITaskGenerator>(
                    new ArithmeticTaskGenerator(genSeed(random)));
                }),
            std::make_pair("config", [this](const StringCol &filenames) {
                base::FileConfig fc(filenames[0]);
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(genSeed(random),
                        readTasksFromConfig(fc.read())));
                }),
            std::make_pair("tagaini", [this](const StringCol &filenames) {
                if(filenames.size() != 1)
                    throw std::runtime_error("filenames param mismatch");
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(genSeed(random),
                        readTagaini(filenames[0])));
                }),
            std::make_pair("tagaini_rev", [this](const StringCol &filenames) {
                if(filenames.size() != 1)
                    throw std::runtime_error("filenames param mismatch");
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(genSeed(random),
                        readTagaini(filenames[0]), true));
                }),
            std::make_pair("tagaini_prons", [this](const StringCol &filenames) {
                if(filenames.size() != 1)
                    throw std::runtime_error("filenames param mismatch");
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(genSeed(random),
                        readTagainiWithProns(filenames[0])));
                }),
            std::make_pair("tagaini_prons_rev", [this](const StringCol &filenames) {
                if(filenames.size() != 1)
                    throw std::runtime_error("filenames param mismatch");
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(genSeed(random),
                        readTagainiWithProns(filenames[0]), true));
                }),
            std::make_pair("multiletter", [this](const StringCol &filenames) {
                if(filenames.size() != 1) throw std::runtime_error("filenames param mismatch");
                base::FileConfig fc(filenames[0]);
                return std::unique_ptr<core::ITaskGenerator>(
                    new MultiLetterTaskGenerator(genSeed(random),
                        readMultiLetterTasksFromConfig(fc.read())));
                }),
            std::make_pair("tagaini_typing", [this](const StringCol &filenames) {
                if(filenames.size() != 1)
                    throw std::runtime_error("filenames param mismatch");
                return std::unique_ptr<core::ITaskGenerator>(
                    new DictTaskGenerator(genSeed(random),
                        readTagainiPronouns(filenames[0])));
                }),
            std::make_pair("wordgen", wordgen::createTaskGenerator),
            std::make_pair("wiktionary", [this](const StringCol &filenames){
                    if(filenames.size() != 1)
                        throw std::runtime_error("filenames param mismatch");
                    std::wifstream stream(filenames[0]);
                    stream.imbue(std::locale(std::locale("")));
                    if(!stream.is_open())
                        throw std::runtime_error("can't read file");
                    return std::unique_ptr<core::ITaskGenerator>(
                        new DictTaskGenerator(genSeed(random),
                            inner::wiktionary::readWiktionaryDefinitions(stream)));
                }),
            std::make_pair("mastermind", [this](const StringCol&) {
                    return std::make_unique<MastermindTaskGenerator>();
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
                return iter->second(task.filenames);
            }
        }
        throw std::invalid_argument("invalid task name");
    }
}
