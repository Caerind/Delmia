#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "../NodeEngine/Core/SpriteComponent.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

#include "Entity.hpp"

class Building : public Entity
{
    public:
        typedef std::shared_ptr<Building> Ptr;

    public:
        Building();
        ~Building();

        void setCoords(int x, int y);
        sf::Vector2i getCoords();

        void addSprite(int x, int y, sf::IntRect rect);

        // TODO : Collisions functions

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        sf::Vector2i mCoords;
        std::vector<std::pair<sf::Vector2i,NSpriteComponent*>> mSprites;
};

#endif // BUILDING_HPP
