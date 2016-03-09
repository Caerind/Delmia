#include "ActionTarget.hpp"
#include "World.hpp"

NActionTarget::NActionTarget()
{
}

void NActionTarget::bind(std::string const& id, NActionTarget::ActionCallback function)
{
    mFunctions[id] = function;
}

void NActionTarget::unbind(std::string const& id)
{
    mFunctions.remove(id);
}

bool NActionTarget::isActive(std::string const& id)
{
    if (mActions.contains(id))
    {
        if (mActions[id].isRealtime())
        {
            return mActions[id].test();
        }
        else
        {
            return NWorld::testAction(mActions[id]);
        }
    }
    return false;
}

void NActionTarget::tick(sf::Time dt)
{
    for (auto itr = mFunctions.begin(); itr != mFunctions.end(); itr++)
    {
        if (isActive(itr->first) && itr->second)
        {
            itr->second(dt);
        }
    }
}
