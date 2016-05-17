#ifndef NBOXCOMPONENT_HPP
#define NBOXCOMPONENT_HPP

#include "CollisionComponent.hpp"
#include <SFML/Graphics/Rect.hpp>

class NBoxComponent : public NCollisionComponent
{
    public:
        NBoxComponent();

        void setOrigin(sf::Vector2f const& origin);
        void setSize(sf::Vector2f const& size);

        sf::Vector2f getOrigin() const;
        sf::Vector2f getSize() const;

        sf::FloatRect getBox() const;
        sf::FloatRect getFinalBox() const;
};

#endif // NBOXCOMPONENT_HPP
