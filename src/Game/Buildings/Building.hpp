#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/MapUtility.hpp"

#include "../Entity.hpp"

class Building : public Entity
{
    public:
        typedef std::shared_ptr<Building> Ptr;

    public:
        Building();
        ~Building();

        virtual std::vector<sf::Vector2i> getTiles(int x, int y);
        virtual void generateBuilding(int x, int y, sf::IntRect rect);
        void addTile(int x, int y, sf::IntRect rect);

        sf::FloatRect getBounds();
        bool collide(int x, int y);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

        struct TileBuilding
        {
            sf::Vector2i coords;
            NSpriteComponent* sprite;
        };

    protected:
        std::vector<TileBuilding> mTiles;
};

#endif // BUILDING_HPP
