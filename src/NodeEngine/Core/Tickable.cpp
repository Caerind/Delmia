#include "Tickable.hpp"
#include "World.hpp"

NTickable::NTickable()
{
    NWorld::instance().addTickable(this);
}

NTickable::~NTickable()
{
    NWorld::instance().removeTickable(this);
}

void NTickable::tick(sf::Time dt)
{
}
