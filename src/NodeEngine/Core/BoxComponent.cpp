#include "BoxComponent.hpp"

NBoxComponent::NBoxComponent()
{
    setPointCount(4);
}

void NBoxComponent::setSize(sf::Vector2f const& size)
{
    setPoint(1,sf::Vector2f(size.x,0.f));
    setPoint(2,size);
    setPoint(3,sf::Vector2f(0.f,size.y));
}

sf::Vector2f NBoxComponent::getSize() const
{
    return getPoint(2);
}

sf::FloatRect NBoxComponent::getBox() const
{
    return sf::FloatRect(getOrigin(),getSize());
}

sf::FloatRect NBoxComponent::getFinalBox() const
{
    return getFinalTransform().transformRect(getBox());
}
