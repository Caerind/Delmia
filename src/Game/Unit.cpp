#include "Unit.hpp"

Unit::Unit()
{
    attachComponent(&mSprite);

    mSprite.setTexture(NWorld::getResources().getTexture("unit"));
    mSprite.setOrigin(64,100);
}

void Unit::load(pugi::xml_node& node)
{
}

void Unit::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Unit";
}
