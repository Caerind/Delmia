#include "CollisionComponent.hpp"

NCollisionComponent::NCollisionComponent()
{
}

void NCollisionComponent::setPoint(std::size_t index, sf::Vector2f const& position)
{
    if (0 <= index && index < mPoints.size())
    {
        mPoints[index] = position;
    }
}

sf::Vector2f NCollisionComponent::getPoint(std::size_t index) const
{
    if (0 <= index && index < mPoints.size())
    {
        return mPoints.at(index);
    }
    return sf::Vector2f();
}

void NCollisionComponent::setPointCount(std::size_t index)
{
    mPoints.resize(index);
}

std::size_t NCollisionComponent::getPointCount() const
{
    return mPoints.size();
}

bool NCollisionComponent::contains(sf::Vector2f const& position)
{
    // TODO : Collision
    return false;
}

bool NCollisionComponent::intersect(NCollisionComponent* component)
{
    // TODO : Collision
    return false;
}

