#include "Tickable.hpp"
#include "World.hpp"

NTickable::NTickable()
{
    mTicked = false;
    NWorld::instance().addTickable(this);
}

NTickable::~NTickable()
{
    NWorld::instance().removeTickable(this);
}

void NTickable::tick(sf::Time dt)
{
}

bool NTickable::hasTicked() const
{
    return mTicked;
}

void NTickable::ticked()
{
    mTicked = true;
}

void NTickable::unticked()
{
    mTicked = false;
}
