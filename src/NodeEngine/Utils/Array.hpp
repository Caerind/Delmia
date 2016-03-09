#ifndef NARRAY_HPP
#define NARRAY_HPP

#include <algorithm>
#include <functional>
#include <vector>

#include "Math.hpp"

template <typename T>
class NArray : public std::vector<T>
{
    public:
        NArray();
        NArray(std::vector<T> const& vector);

        void sort(std::function<bool(T const& a, T const& b)> function);

        void add(T const& element);
        void remove(T const& element);

        void append(NArray<T> const& array);
        void append(std::vector<T> const& vector);

        void erase(std::size_t index);

        bool contains(T const& element);

        T randomElement();
};

template <typename T>
NArray<T>::NArray()
{
}

template <typename T>
NArray<T>::NArray(std::vector<T> const& vector)
{
    append(vector);
}

template <typename T>
void NArray<T>::sort(std::function<bool(T const& a, T const& b)> function)
{
    std::sort(std::vector<T>::begin(),std::vector<T>::end(),function);
}

template <typename T>
void NArray<T>::add(T const& element)
{
    std::vector<T>::push_back(element);
}

template <typename T>
void NArray<T>::remove(T const& element)
{
    for (auto itr = std::vector<T>::begin(); itr != std::vector<T>::end(); itr++)
    {
        if (*itr == element)
        {
            std::vector<T>::erase(itr);
            return;
        }
    }
}

template <typename T>
void NArray<T>::append(NArray<T> const& array)
{
    for (auto itr = array.begin(); itr != array.end(); itr++)
    {
        add(*itr);
    }
}

template <typename T>
void NArray<T>::append(std::vector<T> const& vector)
{
    std::size_t size = vector.size();
    for (std::size_t i = 0; i < size; i++)
    {
        add(vector[i]);
    }
}

template <typename T>
void NArray<T>::erase(std::size_t index)
{
    if (index >= 0 && index < std::vector<T>::size())
    {
        std::vector<T>::erase(std::vector<T>::begin() + index);
    }
}

template <typename T>
bool NArray<T>::contains(T const& element)
{
    return std::find(std::vector<T>::begin(),std::vector<T>::end(),element) != std::vector<T>::end();
}

template <typename T>
T NArray<T>::randomElement()
{
    return std::vector<T>::at(NMath::random(0,(int)std::vector<T>::size()-1));
}

#endif // NARRAY_HPP
