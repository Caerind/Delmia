#ifndef ISOMETRICBASE_HPP
#define ISOMETRICBASE_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/Isometric.hpp"

class IsometricBase
{
    public:
        IsometricBase(sf::Vector2i const& coords = sf::Vector2i());
        ~IsometricBase();

        void setCoords(sf::Vector2i const& coords);
        sf::Vector2i getCoords() const;

        void addTile(sf::Vector2i const& coords, std::string const& textureId, sf::IntRect const& textureRect);
        void removeTile(sf::Vector2i const& coords);
        void clearTiles();

        bool hasTile(sf::Vector2i const& coords);

    protected:
        struct TileBuilding
        {
            sf::Vector2i coords;
            NSpriteComponent sprite;
        };

    protected:
        sf::Vector2i mCoords;
        std::vector<TileBuilding> mTiles;
};

#endif // ISOMETRICBASE_HPP
