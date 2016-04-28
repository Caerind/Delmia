#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../../Game.hpp"

#include "../../Game/Buildings/Buildings.hpp"

class GameState : public ah::State
{
    public:
        GameState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);

        void handlePlacement(sf::Event const& event);
        void movePlacement(sf::Vector2i const& coords);

    protected:
        World mWorld;

        bool mPlacing;
        bool mPlacingCollide;
        int mPlacingType;
        Building::Ptr mPlacement;
};


#endif // GAMESTATE_HPP
