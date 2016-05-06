#include "Transformable.hpp"
#include "World.hpp"

NTransformable::NTransformable()
: mTransformable()
, mZ(0.f)
{
}

sf::Vector2f NTransformable::getPosition() const
{
    return mTransformable.getPosition();
}

void NTransformable::setPosition(sf::Vector2f const& position)
{
    mTransformable.setPosition(position);
    onMoved();
    NWorld::needUpdateOrder();
}

void NTransformable::setPosition(float x, float y)
{
    setPosition({x,y});
}

void NTransformable::move(sf::Vector2f const& movement)
{
    setPosition(movement + getPosition());
}

void NTransformable::move(float x, float y)
{
    move({x,y});
}

sf::Vector2f NTransformable::getOrigin() const
{
    return mTransformable.getOrigin();
}

void NTransformable::setOrigin(sf::Vector2f const& origin)
{
    mTransformable.setOrigin(origin);
    onMoved();
    NWorld::needUpdateOrder();
}

void NTransformable::setOrigin(float x, float y)
{
    setOrigin({x,y});
}

float NTransformable::getPositionZ() const
{
    return mZ;
}

void NTransformable::setPositionZ(float z)
{
    mZ = z;
    onMoved();
    NWorld::needUpdateOrder();
}

void NTransformable::moveZ(float z)
{
    setPositionZ(mZ + z);
}

sf::Vector2f NTransformable::getScale() const
{
    return mTransformable.getScale();
}

void NTransformable::setScale(sf::Vector2f const& scale)
{
    mTransformable.setScale(scale);
    onScaled();
}

void NTransformable::setScale(float x, float y)
{
    setScale({x,y});
}

float NTransformable::getRotation() const
{
    return mTransformable.getRotation();
}

void NTransformable::setRotation(float rotation)
{
    mTransformable.setRotation(rotation);
    onRotated();
}

sf::Transform NTransformable::getTransform() const
{
    return mTransformable.getTransform();
}

void NTransformable::onMoved()
{
}

void NTransformable::onScaled()
{
}

void NTransformable::onRotated()
{
}
