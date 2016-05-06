#include "Component.hpp"

NComponent::NComponent()
{
    mParent = nullptr;
}

void NComponent::attachComponent(NComponent* component)
{
    if (component)
    {
        component->mParent = this;
        add(mComponents,component);
    }
}

void NComponent::detachComponent(NComponent* component)
{
    if (component)
    {
        component->mParent = nullptr;
    }
    remove(mComponents,component);
}

float NComponent::getFinalZ() const
{
    return (mParent != nullptr) ? getPositionZ() + mParent->getFinalZ() : getPositionZ();
}

sf::Vector2f NComponent::getFinalPosition() const
{
    return getFinalTransform().transformPoint(0,0);
}

sf::Transform NComponent::getFinalTransform() const
{
    return (mParent != nullptr) ? mParent->getFinalTransform() * getTransform() : getTransform();
}

sf::Vector2f NComponent::getActorPosition() const
{
    return (mParent != nullptr) ? mParent->getActorPosition() : getPosition();
}

sf::Vector2f NComponent::getActorScale() const
{
    return (mParent != nullptr) ? mParent->getActorScale() : getScale();
}

float NComponent::getActorRotation() const
{
    return (mParent != nullptr) ? mParent->getActorRotation() : getRotation();
}

void NComponent::setActorPosition(sf::Vector2f const& position)
{
    (mParent != nullptr) ? mParent->setActorPosition(position) : setPosition(position);
}

void NComponent::setActorScale(sf::Vector2f const& scale)
{
    (mParent != nullptr) ? mParent->setActorScale(scale) : setPosition(scale);
}

void NComponent::setActorRotation(float rotation)
{
    (mParent != nullptr) ? mParent->setActorRotation(rotation) : setRotation(rotation);
}

void NComponent::onMoved()
{
    onMovedThis();
    onMovedChildren();
}

void NComponent::onMovedThis()
{
}

void NComponent::onMovedChildren()
{
    for (std::size_t i = 0; i < mComponents.size(); i++)
    {
        mComponents[i]->onMoved();
    }
}

void NComponent::onScaled()
{
    onScaledThis();
    onScaledChildren();
}

void NComponent::onScaledThis()
{
}

void NComponent::onScaledChildren()
{
    for (std::size_t i = 0; i < mComponents.size(); i++)
    {
        mComponents[i]->onScaled();
    }
}

void NComponent::onRotated()
{
    onRotatedThis();
    onRotatedChildren();
}

void NComponent::onRotatedThis()
{
}

void NComponent::onRotatedChildren()
{
    for (std::size_t i = 0; i < mComponents.size(); i++)
    {
        mComponents[i]->onRotated();
    }
}

void NComponent::load(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.child(name.c_str());
    setPosition(NString::toVector2f(n.attribute("pos").value()));
    setOrigin(NString::toVector2f(n.attribute("ori").value()));
    setScale(NString::toVector2f(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());
}

void NComponent::save(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.append_child(name.c_str());
    n.append_attribute("pos") = NString::toString(getPosition()).c_str();
    n.append_attribute("ori") = NString::toString(getOrigin()).c_str();
    n.append_attribute("sca") = NString::toString(getScale()).c_str();
    n.append_attribute("rot") = getRotation();
}
