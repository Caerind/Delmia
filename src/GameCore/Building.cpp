#include "Building.hpp"

Building::Building()
{
    setCoords(0,0);
}

Building::~Building()
{
    for (std::size_t i = 0; i < mSprites.size(); i++)
    {
        delete mSprites[i].second;
    }
    mSprites.clear();
}

void Building::setCoords(int x, int y)
{
    mCoords = sf::Vector2i(x,y);
    setPosition(NMapUtility::Isometric::coordsToWorld(mCoords) - NMapUtility::Isometric::coordsToWorld(sf::Vector2i(0,0)));
}

sf::Vector2i Building::getCoords()
{
    return mCoords;
}

void Building::addSprite(int x, int y, sf::IntRect rect)
{
    sf::Vector2i coords = sf::Vector2i(x,y);

    std::pair<sf::Vector2i,NSpriteComponent*> pair;
    pair.first = coords;
    pair.second = new NSpriteComponent();
    pair.second->setTexture("building",rect);
    pair.second->setOrigin(128,192);

    pair.second->setPosition(NMapUtility::Isometric::coordsToWorld(coords));

    attachComponent(pair.second);

    mSprites.push_back(pair);
}

void Building::load(pugi::xml_node& node)
{
    std::size_t size = node.attribute("size").as_uint();
    for (std::size_t i = 0; i < size; i++)
    {
        std::string name = "Sprite" + std::to_string(i);
        pugi::xml_node n = node.child(name.c_str());
        sf::Vector2i coords = NVector::NToSFML2I(NString::toVector(n.attribute("coords").value()));
        sf::IntRect rect = NString::toIntRect(n.attribute("rect").value());
        addSprite(coords.x,coords.y,rect);
    }
}

void Building::save(pugi::xml_node& node)
{
    node.append_attribute("type") = "Building";
    node.append_attribute("size") = mSprites.size();
    for (std::size_t i = 0; i < mSprites.size(); i++)
    {
        std::string name = "Sprite" + std::to_string(i);
        pugi::xml_node n = node.append_child(name.c_str());
        n.append_attribute("coords") = NString::toString(NVector::SFML2IToN(mSprites[i].first)).c_str();
        n.append_attribute("rect") = NString::toString(mSprites[i].second->getTextureRect()).c_str();
    }
}
