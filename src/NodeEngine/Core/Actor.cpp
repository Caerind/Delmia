#include "Actor.hpp"

NActor::NActor()
{
    mParent = nullptr;

    static int x = 0;
    x++;
    mId = std::to_string(x);
}

void NActor::load(pugi::xml_node& node)
{
    setPosition(NString::toVector(node.attribute("pos").value()));
    setScale(NString::toVector(node.attribute("sca").value()));
    setRotation(node.attribute("rot").as_float());
}

void NActor::save(pugi::xml_node& node)
{
    node.append_attribute("pos") = NString::toString(getPosition()).c_str();
    node.append_attribute("sca") = NString::toString(getScale()).c_str();
    node.append_attribute("rot") = getRotation();
}

std::string NActor::getId() const
{
    return mId;
}
