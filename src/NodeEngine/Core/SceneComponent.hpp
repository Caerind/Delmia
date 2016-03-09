#ifndef NSCENECOMPONENT_HPP
#define NSCENECOMPONENT_HPP

#include "Component.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

class NSceneComponent : public NComponent
{
    public:
        NSceneComponent();
        ~NSceneComponent();

        virtual void render(sf::RenderTarget& target);
};

#endif // NSCENECOMPONENT_HPP
