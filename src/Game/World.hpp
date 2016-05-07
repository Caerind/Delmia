#ifndef WORLD_HPP
#define WORLD_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/Isometric.hpp"

#include "../Game.hpp"
#include "Map/Map.hpp"
#include "Buildings/Building.hpp"
#include "Resources/Resource.hpp"
#include "Units/Unit.hpp"

class World
{
    public:
        World();

        void handleEvent(sf::Event const& event);
        void update(sf::Time dt);
        void render(sf::RenderTarget& target);

        sf::Vector2i getMouseCoords();

        bool collide(int x, int y, bool isSolid = true);

        std::vector<Unit::Ptr> selectUnits(sf::FloatRect const& zone);

        void removeActor(std::string const& id);

        void clear();

        template <typename T>
        bool buildingPlacing(int x, int y);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createBuilding(int x, int y, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createResource(int x, int y, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createUnit(float x, float y, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createActor(Args&& ... args);

        Map::Ptr getMap();

    protected:
        std::shared_ptr<Map> mMap;
        std::map<std::string,std::shared_ptr<Building>> mBuildings;
        std::map<std::string,std::shared_ptr<Resource>> mResources;
        std::map<std::string,std::shared_ptr<Unit>> mUnits;
};

template <typename T>
bool World::buildingPlacing(int x, int y)
{
    std::vector<sf::Vector2i> tiles = T::getTilesBlueprint(x,y);
    for (sf::Vector2i const& t : tiles)
    {
        if (collide(t.x,t.y))
        {
            return false;
        }
        std::vector<sf::Vector2i> neighboors = NIsometric::getNeighboors(t,true);
        for (sf::Vector2i const& n : neighboors)
        {
            if (collide(n.x,n.y,false))
            {
                return false;
            }
        }
        for (auto itr = mUnits.begin(); itr != mUnits.end(); itr++)
        {
            if (itr->second->getCoords() == t)
            {
                return false;
            }
        }
    }
    return true;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createBuilding(int x, int y, Args&& ... args)
{
    if (!buildingPlacing<T>(x,y))
    {
        return nullptr;
    }

    std::shared_ptr<T> actor = NWorld::createActor<T>(x,y,std::forward<Args>(args)...);
    mBuildings[actor->getId()] = actor;

    for (auto itr = mUnits.begin(); itr != mUnits.end(); itr++)
    {
        itr->second->onBuildingAdded(T::getTilesBlueprint(x,y));
    }

    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createResource(int x, int y, Args&& ... args)
{
    if (collide(x,y))
    {
        return nullptr;
    }

    std::shared_ptr<T> actor = NWorld::createActor<T>(x,y,std::forward<Args>(args)...);
    mResources[actor->getId()] = actor;

    for (auto itr = mUnits.begin(); itr != mUnits.end(); itr++)
    {
        itr->second->onBuildingAdded(T::getTilesBlueprint(x,y));
    }

    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createUnit(float x, float y, Args&& ... args)
{
    std::shared_ptr<T> actor = NWorld::createActor<T>(x,y,std::forward<Args>(args)...);
    mUnits[actor->getId()] = actor;
    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createActor(Args&& ... args)
{
    return NWorld::createActor<T>(std::forward<Args>(args)...);
}

#endif // WORLD_HPP
