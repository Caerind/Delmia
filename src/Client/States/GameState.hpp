#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../../NodeEngine/Core/World.hpp"
#include "../../NodeEngine/Core/MapUtility.hpp"

#include "../../Game.hpp"

class GameState : public ah::State
{
    public:
        GameState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);
};

#endif // GAMESTATE_HPP
