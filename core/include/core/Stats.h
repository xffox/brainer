#ifndef CORE_STATS_H
#define CORE_STATS_H

namespace core
{
    class Stats
    {
    public:
        Stats()
            :taskCount(0), validCount(0), triesCount(0), averageTime(0.0)
        {}

        unsigned int getTaskCount() const
        {
            return taskCount;
        }

        Stats &setTaskCount(unsigned int taskCount)
        {
            this->taskCount = taskCount;
            return *this;
        }

        unsigned int getValidCount() const
        {
            return validCount;
        }

        Stats &setValidCount(unsigned int validCount)
        {
            this->validCount = validCount;
            return *this;
        }

        unsigned int getTriesCount() const
        {
            return triesCount;
        }

        Stats &setTriesCount(unsigned int triesCount)
        {
            this->triesCount = triesCount;
            return *this;
        }

        double getAverageTime() const
        {
            return averageTime;
        }

        Stats &setAverageTime(double averageTime)
        {
            this->averageTime = averageTime;
            return *this;
        }

    private:
        unsigned int taskCount;
        unsigned int validCount;
        unsigned int triesCount;
        double averageTime;
    };
}

#endif
