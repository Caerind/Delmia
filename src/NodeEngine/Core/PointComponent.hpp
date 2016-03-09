#ifndef NPOINTCOMPONENT_HPP
#define NPOINTCOMPONENT_HPP

#include <SFML/Graphics/CircleShape.hpp>

#include "SceneComponent.hpp"

class NPointComponent : public NSceneComponent
{
    public:
        NPointComponent();

        void render(sf::RenderTarget& target);

    private:
        sf::CircleShape mPoint;
};

#endif // NPOINTCOMPONENT_HPP
