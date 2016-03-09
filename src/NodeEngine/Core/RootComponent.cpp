#include "RootComponent.hpp"
#include "Actor.hpp"

NRootComponent::NRootComponent(NActor* actor)
{
    mParent = nullptr;
    mActor = actor;
}

void NRootComponent::load(pugi::xml_node& node, std::string const& name)
{
    setPosition(NString::toVector(node.attribute("pos").value()));
    setScale(NString::toVector(node.attribute("sca").value()));
    setRotation(node.attribute("rot").as_float());
}

void NRootComponent::save(pugi::xml_node& node, std::string const& name)
{
    node.append_attribute("pos") = NString::toString(getPosition()).c_str();
    node.append_attribute("sca") = NString::toString(getScale()).c_str();
    node.append_attribute("rot") = getRotation();
}
