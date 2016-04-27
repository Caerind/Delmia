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

        static std::vector<sf::Vector2i> getTilesBlueprint(int x, int y);
        virtual std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(int x, int y) = 0;
        virtual void generate(int x, int y);
        void addTile(int x, int y, sf::IntRect rect);

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
