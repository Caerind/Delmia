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

    protected:
        World mWorld;
        int mType;
};


#endif // GAMESTATE_HPP
