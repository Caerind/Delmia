#ifndef NLAYERCOMPONENT_HPP
#define NLAYERCOMPONENT_HPP

#include <vector>

#include <SFML/Graphics/Sprite.hpp>

#include "MapUtility.hpp"
#include "SceneComponent.hpp"

class NLayerComponent : public NSceneComponent
{
    public:
        NLayerComponent();

        void create(std::string const& textureName, sf::Vector2i mapSize, sf::Vector2i tileSize, NMapUtility::Type::MapType type = NMapUtility::Type::Orthogonal, int hexSide = 0);

        bool loadFromCode(std::string const& code);
        std::string getCode() const;

        sf::Vector2i getMapSize() const;
        sf::Vector2i getTileSize() const;

        void render(sf::RenderTarget& target);

        sf::FloatRect getBounds() const;
        bool contains(sf::Vector2f const& position) const;

        NMapUtility::Type::MapType getType() const;
        bool isOrthogonal() const;
        bool isIsometric() const;
        bool isHexagonal() const;

        void setTileId(sf::Vector2i const& coords, int id);
        int getTileId(sf::Vector2i const& coords) const;

        sf::IntRect idToRect(int id) const;
        int rectToId(sf::IntRect const& rect) const;

        virtual void load(pugi::xml_node& node, std::string const& name = "LayerComponent");
        virtual void save(pugi::xml_node& node, std::string const& name = "LayerComponent");

    private:
        std::vector<sf::Sprite> mTiles;
        std::string mTexture;
        sf::Vector2i mMapSize;
        sf::Vector2i mTileSize;
        NMapUtility::Type::MapType mType;
        int mHexSide;
};

#endif // NLAYERCOMPONENT_HPP
