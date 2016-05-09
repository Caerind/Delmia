#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "../../NodeEngine/Core/SpriteComponent.hpp"
#include "../../NodeEngine/Core/Isometric.hpp"

#include "../Entity.hpp"

#include "../Buildings/Buildings.hpp"
#include "Resources.hpp"

class Resource : public Entity
{
    public:
        typedef std::shared_ptr<Resource> Ptr;

    public:
        Resource();
        Resource(sf::Vector2i const& coords);

        std::size_t getType() const;
        sf::Vector2i getCoords() const;
        bool collide(sf::Vector2i const& coords) const;

        void initTile(sf::IntRect const& textureRect);

    protected:
        std::size_t mType;
        sf::Vector2i mCoords;
        NSpriteComponent mSprite;
};

#endif // RESOURCE_HPP
