#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/Isometric.hpp"

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
        Building(sf::Vector2i const& coords);
        ~Building();

        std::size_t getType() const;

        sf::Vector2i getCoords() const;

        sf::Color getColor() const;
        void setColor(sf::Color const& color);

        static std::vector<sf::Vector2i> getTilesBlueprint(sf::Vector2i const& coords);
        virtual std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(sf::Vector2i const& coords) = 0;
        void addTile(sf::Vector2i const& coords, sf::IntRect rect);

        void generate();
        void generate(sf::Vector2i const& coords);

        bool collide(sf::Vector2i const& coords);

        void setBuilt(bool built);
        void build(float unit);
        virtual void onBuildEnded();
        bool isBuilt() const;

        void clearTiles();

        void tick(sf::Time dt);

        void load(pugi::xml_node& node);
        void save(pugi::xml_node& node);

    protected:
        std::size_t mType;
        sf::Vector2i mCoords;
        sf::Color mColor;
        std::vector<TileBuilding> mTiles;
        bool mBuilt;
};

#endif // BUILDING_HPP
