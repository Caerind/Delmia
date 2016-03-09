#ifndef NMAP_HPP
#define NMAP_HPP

#include <algorithm>
#include <functional>
#include <map>

template <typename K, typename T>
class NMap : public std::map<K,T>
{
    public:
        NMap();

        void add(K const& key, T const& element);
        void remove(K const& key);

        void append(NMap<K,T> const& map);
        void append(std::map<K,T> const& map);

        bool contains(K const& key);
};

template <typename K, typename T>
NMap<K,T>::NMap()
{
}

template <typename K, typename T>
void NMap<K,T>::add(K const& key, T const& element)
{
    std::map<K,T>::insert(std::pair<K,T>(key,element));
}

template <typename K, typename T>
void NMap<K,T>::remove(K const& key)
{
    auto itr = std::map<K,T>::find(key);
    if (itr != std::map<K,T>::end())
    {
        std::map<K,T>::erase(itr);
    }
}

template <typename K, typename T>
void NMap<K,T>::append(NMap<K,T> const& array)
{
    for (auto itr = array.begin(); itr != array.end(); itr++)
    {
        add(itr->first, itr->second);
    }
}

template <typename K, typename T>
void NMap<K,T>::append(std::map<K,T> const& map)
{
    for (auto itr = map.begin(); itr != map.end(); itr++)
    {
        add(itr->first, itr->second);
    }
}

template <typename K, typename T>
bool NMap<K,T>::contains(K const& key)
{
    return (std::map<K,T>::find(key) != std::map<K,T>::end());
}

#endif // NMAP_HPP
