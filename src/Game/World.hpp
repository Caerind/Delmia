#ifndef WORLD_HPP
#define WORLD_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

#include "../Game.hpp"
#include "Buildings/Building.hpp"
#include "Units/Unit.hpp"

class World
{
    public:
        World();

        void handleEvent(sf::Event const& event);
        void update(sf::Time dt);
        void render(sf::RenderTarget& target);

        sf::Vector2i getMouseCoords();
        bool collide(int x, int y);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createActor(Args&& ... args);

    protected:
        std::shared_ptr<Map> mMap;
        std::map<std::string,std::shared_ptr<Building>> mBuildings;
        std::map<std::string,std::shared_ptr<Unit>> mUnits;
};

template <typename T, typename ... Args>
std::shared_ptr<T> World::createActor(Args&& ... args)
{
    std::shared_ptr<T> actor = std::make_shared<T>(std::forward<Args>(args)...);
    std::shared_ptr<NActor> nactor = std::dynamic_pointer_cast<NActor>(actor);
    std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(actor);
    std::shared_ptr<Unit> unit = std::dynamic_pointer_cast<Unit>(actor);
    
    if(nactor)
    {
      NWorld::addActor(nactor);
      NLog::log("NAcotr added");
    }
    
    if(building)
    {
        mBuildings[actor->getId()] = building;
        NLog::log("Building added");
    }
    
    if(unit)
    {
        mUnits[actor->getId()] = unit;
        NLog::log("Unit added");
    }
    return actor;
}

#endif // WORLD_HPP
