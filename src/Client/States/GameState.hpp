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

        template <typename T, typename ... Args>
        std::shared_ptr<T> createActor(std::string str,Args&& ... args);

    protected:
        std::map<std::string, std::shared_ptr<Building>> mBuilding;
        std::map<std::string, std::shared_ptr<Unit>> mUnit;
};

#endif // GAMESTATE_HPP
