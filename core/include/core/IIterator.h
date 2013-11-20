#ifndef CORE_IITERATOR_H
#define CORE_IITERATOR_H

namespace core
{
    template<typename T>
    class IIterator
    {
    public:
        virtual ~IIterator(){}

        virtual const T *next() = 0;
    };
}

#endif
