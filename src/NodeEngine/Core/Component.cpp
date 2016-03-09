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
        mComponents.add(component);
    }
}

void NComponent::detachComponent(NComponent* component)
{
    if (component)
    {
        component->mParent = nullptr;
    }
    mComponents.remove(component);
}

float NComponent::getFinalZ() const
{
    if (mParent != nullptr)
    {
        return getPosition().z + mParent->getFinalZ();
    }
    else
    {
        return getPosition().z;
    }
}

NVector NComponent::getFinalPosition() const
{
    NVector v = NVector::SFML2FToN(getFinalTransform().transformPoint(0,0));
    v.z = getFinalZ();
    return v;
}

sf::Transform NComponent::getFinalTransform() const
{
    if (mParent != nullptr)
    {
        return mParent->getFinalTransform() * getTransform();
    }
    else
    {
        return getTransform();
    }
}

NVector NComponent::getParentPosition() const
{
    if (mParent != nullptr)
    {
        return mParent->getPosition();
    }
    else
    {
        return NVector();
    }
}

NVector NComponent::getParentScale() const
{
    if (mParent != nullptr)
    {
        return mParent->getScale();
    }
    else
    {
        return NVector();
    }
}

float NComponent::getParentRotation() const
{
    if (mParent != nullptr)
    {
        return mParent->getRotation();
    }
    else
    {
        return 0.f;
    }
}

NVector NComponent::getActorPosition() const
{
    if (mParent != nullptr)
    {
        return mParent->getActorPosition();
    }
    else
    {
        return getPosition();
    }
}

NVector NComponent::getActorScale() const
{
    if (mParent != nullptr)
    {
        return mParent->getActorScale();
    }
    else
    {
        return getScale();
    }
}

float NComponent::getActorRotation() const
{
    if (mParent != nullptr)
    {
        return mParent->getActorRotation();
    }
    else
    {
        return getRotation();
    }
}

void NComponent::setActorPosition(NVector const& position)
{
    if (mParent != nullptr)
    {
        mParent->setActorPosition(position);
    }
    else
    {
        setPosition(position);
    }
}

void NComponent::setActorScale(NVector const& scale)
{
    if (mParent != nullptr)
    {
        mParent->setActorScale(scale);
    }
    else
    {
        setPosition(scale);
    }
}

void NComponent::setActorRotation(float rotation)
{
    if (mParent != nullptr)
    {
        mParent->setActorRotation(rotation);
    }
    else
    {
        setRotation(rotation);
    }
}

void NComponent::load(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.child(name.c_str());
    setPosition(NString::toVector(n.attribute("pos").value()));
    setScale(NString::toVector(n.attribute("sca").value()));
    setRotation(n.attribute("rot").as_float());
}

void NComponent::save(pugi::xml_node& node, std::string const& name)
{
    pugi::xml_node n = node.append_child(name.c_str());
    n.append_attribute("pos") = NString::toString(getPosition()).c_str();
    n.append_attribute("sca") = NString::toString(getScale()).c_str();
    n.append_attribute("rot") = getRotation();
}
