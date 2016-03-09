#ifndef NACTIONMAP_HPP
#define NACTIONMAP_HPP

#include "../Utils/Map.hpp"
#include "Action.hpp"

class NActionMap
{
    public:
        NActionMap();

        template <typename ... Args>
        void setAction(std::string const& id, Args&& ... args);

        NAction getAction(std::string const& id);

        void append(NActionMap map);

    protected:
        NMap<std::string,NAction> mActions;
};

template <typename ... Args>
void NActionMap::setAction(std::string const& id, Args&& ... args)
{
    mActions[id] = NAction(std::forward<Args>(args)...);
}

#endif // NACTIONMAP_HPP
