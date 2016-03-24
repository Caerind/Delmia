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

        template <typename T>
        std::shared_ptr<T> getActor(std::string const& str);

        void removeActor(std::string const& id);


    protected:
        std::vector<std::shared_ptr<Building>> mBuildings;
        std::vector<std::shared_ptr<Unit>> mUnits;
};

template <typename T, typename ... Args>
std::shared_ptr<T> GameState::createActor(Args&& ... args)
{
    std::shared_ptr<T> actor =  std::make_shared<T>(std::forward<Args>(args)...);

    std::shared_ptr<Building> building;
    if(building = std::dynamic_pointer_cast<Building>(actor))
    {
        for(auto it = mBuildings.begin();it!=mBuildings.end();it++)
        {
            if(building->getCoords() == (*it)->getCoords())
            {
                NLog::log("Can't add Building");
                return nullptr;
            }
        }
        mBuildings.push_back(building);
        NLog::log("Building added");
    }
    std::shared_ptr<Unit> unit;
    if(unit = std::dynamic_pointer_cast<Unit>(actor))
    {
        mUnits.push_back(unit);
        NLog::log("Unit added");
    }

    std::shared_ptr<NActor> nactor = std::dynamic_pointer_cast<NActor>(actor);
    if(nactor)
        NWorld::addActor(nactor);
    else
        NLog::log("Can't add Actor");

    return actor;
}

template <typename T>
std::shared_ptr<T> GameState::getActor(std::string const& str)
{
    return std::dynamic_pointer_cast<T>(NWorld::getActor(str));
}

#endif // GAMESTATE_HPP
