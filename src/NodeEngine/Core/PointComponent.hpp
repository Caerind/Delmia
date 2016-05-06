#ifndef NPOINTCOMPONENT_HPP
#define NPOINTCOMPONENT_HPP

#include <SFML/Graphics/CircleShape.hpp>

#include "SceneComponent.hpp"

class NPointComponent : public NSceneComponent
{
    public:
        NPointComponent();

        void setColor(sf::Color const& color);
        sf::Color getColor() const;

        void setRadius(float r);
        float getRadius() const;

        void render(sf::RenderTarget& target);

    private:
        sf::CircleShape mPoint;
};

#endif // NPOINTCOMPONENT_HPP
