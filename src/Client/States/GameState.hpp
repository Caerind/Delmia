#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../../Game/World.hpp"

class GameState : public ah::State
{
    public:
        GameState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);

    protected:
        World mWorld;
};


#endif // GAMESTATE_HPP
