#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "../Entity.hpp"
#include "../IsometricBase.hpp"
#include "../PlayerOwned.hpp"

#include "Buildings.hpp"

class Building : public Entity, public IsometricBase, public PlayerOwned
{
    public:
        typedef std::shared_ptr<Building> Ptr;

    public:
        Building();
        Building(Player* player, sf::Vector2i const& coords);
        ~Building();

        std::size_t getType() const;

        sf::Color getColor() const;
        void setColor(sf::Color const& color);

        static std::vector<sf::Vector2i> getTilesBlueprint(sf::Vector2i const& coords);
        virtual std::vector<std::pair<sf::Vector2i,sf::IntRect>> getTiles(sf::Vector2i const& coords) = 0;

        void generate();
        void generate(sf::Vector2i const& coords);

        void setBuilt(bool built);
        void build(float unit);
        virtual void onBuildEnded();
        bool isBuilt() const;

        void tick(sf::Time dt);

    protected:
        std::size_t mType;
        sf::Color mColor;
        bool mBuilt;
};

#endif // BUILDING_HPP
