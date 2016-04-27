#ifndef BASE_NULLABLE_H
#define BASE_NULLABLE_H

#include <stdexcept>

namespace base
{
    template<typename T>
    class Nullable
    {
    public:
        Nullable();
        Nullable(const T &value);

        Nullable &operator=(const T &value);
        
        T &operator*();
        const T &operator*() const;

        T *operator->();
        const T *operator->() const;

        bool isNull() const
        {
            return nulled;
        }

    private:
        T value;
        bool nulled;
    };

    // return the value or default-constructed value if null
    template<typename T>
    inline T extractNullable(const Nullable<T> &value)
    {
        return !value.isNull()?*value:T();
    }

    template<typename T>
    Nullable<T>::Nullable()
        :value(), nulled(true)
    {
    }

    template<typename T>
    Nullable<T>::Nullable(const T &value)
        :value(value), nulled(false)
    {
    }

    template<typename T>
    Nullable<T> &Nullable<T>::operator=(const T &value)
    {
        this->value = value;
        nulled = false;
        return *this;
    }
        
    template<typename T>
    T &Nullable<T>::operator*()
    {
        if(!nulled)
            return value;
        throw std::invalid_argument("null object");
    }

    template<typename T>
    const T &Nullable<T>::operator*() const
    {
        return const_cast<Nullable<T>*>(this)->operator*();
    }

    template<typename T>
    T *Nullable<T>::operator->()
    {
        return &operator*();
    }

    template<typename T>
    const T *Nullable<T>::operator->() const
    {
        return const_cast<Nullable<T>*>(this)->operator->();
    }
}

#endif
