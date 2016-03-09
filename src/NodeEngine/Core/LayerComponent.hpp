#ifndef NLAYERCOMPONENT_HPP
#define NLAYERCOMPONENT_HPP

#include <vector>

#include <SFML/Graphics/Sprite.hpp>

#include "SceneComponent.hpp"

class NLayerComponent : public NSceneComponent
{
    public:
        NLayerComponent();

        enum Type
        {
            Orthogonal,
            Isometric,
            Hexagonal,
        };

        void create(std::string const& textureName, sf::Vector2i mapSize, sf::Vector2i tileSize, int type = Type::Orthogonal, int hexSide = 0);

        void render(sf::RenderTarget& target);

        sf::FloatRect getBounds() const;
        bool contains(NVector const& position) const;

        int getType() const;
        bool isOrthogonal() const;
        bool isIsometric() const;
        bool isHexagonal() const;

        void setTileId(sf::Vector2i const& coords, int id);
        int getTileId(sf::Vector2i const& coords);

        sf::IntRect idToRect(int id);
        int rectToId(sf::IntRect const& rect);

        virtual void load(pugi::xml_node& node, std::string const& name = "LayerComponent");
        virtual void save(pugi::xml_node& node, std::string const& name = "LayerComponent");

    private:
        std::vector<sf::Sprite> mTiles;
        std::string mTexture;
        sf::Vector2i mMapSize;
        sf::Vector2i mTileSize;
        int mType;
        int mHexSide;
};

#endif // NLAYERCOMPONENT_HPP
