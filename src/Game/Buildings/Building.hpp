#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/MapUtility.hpp"

#include "../Entity.hpp"

#include "Buildings.hpp"

class Building : public Entity
{
    public:
        typedef std::shared_ptr<Building> Ptr;

    protected:
        struct TileBuilding
        {
            sf::Vector2i coords;
            NSpriteComponent* sprite;
        };

    public:
        Building();
        Building(int x, int y);
        ~Building();

        std::size_t getType() const;

        static std::vector<sf::Vector2i> getTilesBlueprint(int x, int y);
        virtual std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(int x, int y) = 0;
        void addTile(int x, int y, sf::IntRect rect);

        virtual void generate();
        virtual void generate(int x, int y);

        bool collide(int x, int y);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

        void setCoords(int x, int y);
        sf::Vector2i getCoords() const;

        void setColor(sf::Color color);
        sf::Color getColor() const;

        virtual void setBuilt(bool built);
        virtual void build(float unit);
        virtual void onBuildEnded();
        bool isBuilt() const;

        void clearTiles();

        void tick(sf::Time dt);

    protected:
        std::size_t mType;
        sf::Vector2i mCoords;
        sf::Color mColor;
        std::vector<TileBuilding> mTiles;
        bool mBuilt;
};

#endif // BUILDING_HPP
