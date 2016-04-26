#ifndef NSCENECOMPONENT_HPP
#define NSCENECOMPONENT_HPP

#include <SFML/Graphics/RenderTarget.hpp>

#include "Component.hpp"

class NSceneComponent : public NComponent
{
    public:
        NSceneComponent();
        virtual ~NSceneComponent();

        virtual void render(sf::RenderTarget& target);
};

#endif // NSCENECOMPONENT_HPP
