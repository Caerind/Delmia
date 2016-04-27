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
        std::shared_ptr<T> createBuilding(int x, int y, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createActor(Args&& ... args);

    protected:
        std::shared_ptr<Map> mMap;
        std::map<std::string,std::shared_ptr<Building>> mBuildings;
        std::map<std::string,std::shared_ptr<Unit>> mUnits;
};

template <typename T, typename ... Args>
std::shared_ptr<T> World::createBuilding(int x, int y, Args&& ... args)
{
<<<<<<< HEAD
    std::vector<sf::Vector2i> tiles = T::getTilesBlueprint(x,y);
    for (auto v : tiles)
=======
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
>>>>>>> 5ea39c1d242c1fbe386f439a558d0625bb54b2ee
    {
        if (collide(v.x,v.y))
        {
            return nullptr;
        }
    }
<<<<<<< HEAD

    std::shared_ptr<T> actor = NWorld::createActor<T>(x,y,std::forward<Args>(args)...);
    mBuildings[actor->getId()] = actor;

    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createActor(Args&& ... args)
{
    std::shared_ptr<T> actor = NWorld::createActor<T>(std::forward<Args>(args)...);

    std::shared_ptr<Unit> unit;
    if(unit = std::dynamic_pointer_cast<Unit>(actor))
=======
    
    if(unit)
>>>>>>> 5ea39c1d242c1fbe386f439a558d0625bb54b2ee
    {
        mUnits[actor->getId()] = unit;
    }
    return actor;
}

#endif // WORLD_HPP
