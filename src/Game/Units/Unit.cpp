#include "Unit.hpp"

Unit::Unit()
{
    attachComponent(&mSprite);

    setPositionZ(0.f);

    mSprite.setTexture(NWorld::getResources().getTexture("unit"));
    mSprite.setOrigin(64,100);
    mSprite.setPositionZ(1.f);
}

void Unit::load(pugi::xml_node& node)
{
}

void Unit::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Unit";
}
