#include "PointComponent.hpp"

NPointComponent::NPointComponent()
{
    mPoint.setRadius(2.f);
    mPoint.setFillColor(sf::Color::Green);
}

void NPointComponent::render(sf::RenderTarget& target)
{
    sf::RenderStates states;
    states.transform *= getFinalTransform();
    target.draw(mPoint,states);
}
