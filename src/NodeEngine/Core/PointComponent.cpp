#include "PointComponent.hpp"

NPointComponent::NPointComponent()
{
    mPoint.setRadius(2.f);
    mPoint.setFillColor(sf::Color::Red);
    mPoint.setOrigin(2.f,2.f);
}

void NPointComponent::setColor(sf::Color const& color)
{
    mPoint.setFillColor(color);
}

sf::Color NPointComponent::getColor() const
{
    return mPoint.getFillColor();
}

void NPointComponent::setRadius(float r)
{
    mPoint.setRadius(r);
    mPoint.setOrigin(r,r);
}

float NPointComponent::getRadius() const
{
    return mPoint.getRadius();
}

void NPointComponent::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform *= getFinalTransform();
    target.draw(mPoint,states);
}
