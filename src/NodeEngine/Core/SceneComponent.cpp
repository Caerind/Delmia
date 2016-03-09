#include "SceneComponent.hpp"
#include "World.hpp"

NSceneComponent::NSceneComponent()
{
    NWorld::instance().addRenderable(this);
}

NSceneComponent::~NSceneComponent()
{
    NWorld::instance().removeRenderable(this);
}

void NSceneComponent::render(sf::RenderTarget& target)
{
}
