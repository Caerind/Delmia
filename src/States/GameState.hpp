#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../NodeEngine/Application/Application.hpp"
#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

#include "../GameCore/Unit.hpp"
#include "../GameCore/Building.hpp"
#include "../GameCore/Map.hpp"
#include "../GameObject/Forest.hpp"

class GameState : public ah::State
{
    public:
        GameState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);
};

#endif // GAMESTATE_HPP
