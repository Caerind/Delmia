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
        std::shared_ptr<T> createActor(Args&& ... args);

    protected:
        std::vector<std::shared_ptr<Building>> mBuildings;
        std::vector<std::shared_ptr<Unit>> mUnits;
};

template <typename T, typename ... Args>
std::shared_ptr<T> GameState::createActor(Args&& ... args)
{
    std::shared_ptr<T> actor = NWorld::createActor<T>(std::forward<Args>(args)...);
    std::shared_ptr<Building> building;
    if(building = std::dynamic_pointer_cast<Building>(actor))
    {
        mBuildings.push_back(building);
        NLog::log("Building added");
    }
    std::shared_ptr<Unit> unit;
    if(unit = std::dynamic_pointer_cast<Unit>(actor))
    {
        mUnits.push_back(unit);
        NLog::log("Unit added");
    }
    return actor;
}

#endif // GAMESTATE_HPP
