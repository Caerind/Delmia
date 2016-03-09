#include "ActionMap.hpp"

NActionMap::NActionMap()
{
    //ctor
}

NAction NActionMap::getAction(std::string const& id)
{
    if (mActions.contains(id))
    {
        return mActions.at(id);
    }
    else
    {
        return NAction();
    }
}

void NActionMap::append(NActionMap map)
{
    mActions.append(map.mActions);
}
