#include "Entity.hpp"
#include "World.hpp"

Entity::Entity()
{
    mWorld = nullptr;
    mLife = 100.f;
    mLifeMax = 100.f;
}

void Entity::setLife(float life)
{
    mLife = (life <= mLifeMax) ? life : mLifeMax;
    mLife = (mLife >= 0.f) ? mLife : 0.f;
}

void Entity::setLifeMax(float lifeMax)
{
    mLifeMax = lifeMax;
}

float Entity::getLife() const
{
    return mLife;
}

float Entity::getLifeMax() const
{
    return mLifeMax;
}

float Entity::getLifePercent() const
{
    return (mLife / mLifeMax) * 100.f;
}

bool Entity::isDead() const
{
    return mLife <= 0.f;
}

void Entity::inflige(float value)
{
    setLife(mLife - value);
}

void Entity::restore(float value)
{
    setLife(mLife + value);
}

void Entity::addResource(std::size_t resourceId, int value)
{
    mResources[resourceId] += value;
}

void Entity::removeResource(std::size_t resourceId, int value)
{
    mResources[resourceId] -= value;
}

int Entity::moveResource(std::size_t resourceId)
{
    int amount = getResourceAmount(resourceId);
    removeResource(resourceId,amount);
    return amount;
}

int Entity::moveResource(std::size_t resourceId, int value)
{
    if (hasResourceAmount(resourceId,value))
    {
        removeResource(resourceId,value);
        return value;
    }
    return 0;
}

bool Entity::hasResource(std::size_t resourceId) const
{
    return (mResources.find(resourceId) != mResources.end());
}

bool Entity::hasResourceAmount(std::size_t resourceId, int value) const
{
    auto itr = mResources.find(resourceId);
    if (itr != mResources.end())
    {
        return (itr->second >= value);
    }
    return false;
}

int Entity::getResourceAmount(std::size_t resourceId) const
{
    auto itr = mResources.find(resourceId);
    if (itr != mResources.end())
    {
        return itr->second;
    }
    return 0;
}

void Entity::setWorld(World* world)
{
    mWorld = world;
}
