#ifndef WORLD_HPP
#define WORLD_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/Isometric.hpp"

#include "../Game.hpp"
#include "Map.hpp"
#include "Buildings/Building.hpp"
#include "Resources/Resource.hpp"
#include "Units/Unit.hpp"

enum Tile
{
    None = 0,
    Dirt,
    Path,
    Water,
};

class World
{
    public:
        World();

        void handleEvent(sf::Event const& event);
        void update(sf::Time dt);
        void render(sf::RenderTarget& target);

        sf::Vector2i getMouseCoords();

        bool collide(sf::Vector2i const& coords, bool isSolid = true);

        std::vector<Unit::Ptr> selectUnits(sf::FloatRect const& zone);

        void removeActor(std::string const& id);

        void clear();

        template <typename T>
        bool buildingPlacing(sf::Vector2i const& coords);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createBuilding(sf::Vector2i const& coords, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createResource(sf::Vector2i const& coords, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createUnit(sf::Vector2f const& pos, Args&& ... args);

        template <typename T, typename ... Args>
        std::shared_ptr<T> createActor(Args&& ... args);

        std::shared_ptr<Map> getMap();

    protected:
        std::shared_ptr<Map> mMap;
        std::map<std::string,std::shared_ptr<Building>> mBuildings;
        std::map<std::string,std::shared_ptr<Resource>> mResources;
        std::map<std::string,std::shared_ptr<Unit>> mUnits;
};

template <typename T>
bool World::buildingPlacing(sf::Vector2i const& coords)
{
    std::vector<sf::Vector2i> tiles = T::getTilesBlueprint(coords);
    for (sf::Vector2i const& t : tiles)
    {
        if (collide(t))
        {
            return false;
        }
        std::vector<sf::Vector2i> neighboors = NIsometric::getNeighboors(t,true);
        for (sf::Vector2i const& n : neighboors)
        {
            if (collide(n,false))
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
std::shared_ptr<T> World::createBuilding(sf::Vector2i const& coords, Args&& ... args)
{
    if (!buildingPlacing<T>(coords))
    {
        return nullptr;
    }

    std::shared_ptr<T> actor = NWorld::createActor<T>(coords,std::forward<Args>(args)...);
    mBuildings[actor->getId()] = actor;
    actor->setWorld(this);

    for (auto itr = mUnits.begin(); itr != mUnits.end(); itr++)
    {
        itr->second->onBuildingAdded(T::getTilesBlueprint(coords));
    }

    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createResource(sf::Vector2i const& coords, Args&& ... args)
{
    if (collide(coords))
    {
        return nullptr;
    }

    std::shared_ptr<T> actor = NWorld::createActor<T>(coords,std::forward<Args>(args)...);
    mResources[actor->getId()] = actor;
    actor->setWorld(this);

    for (auto itr = mUnits.begin(); itr != mUnits.end(); itr++)
    {
        itr->second->onBuildingAdded(T::getTilesBlueprint(coords));
    }

    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createUnit(sf::Vector2f const& pos, Args&& ... args)
{
    std::shared_ptr<T> actor = NWorld::createActor<T>(pos,std::forward<Args>(args)...);
    mUnits[actor->getId()] = actor;
    actor->setWorld(this);
    return actor;
}

template <typename T, typename ... Args>
std::shared_ptr<T> World::createActor(Args&& ... args)
{
    return NWorld::createActor<T>(std::forward<Args>(args)...);
}

#endif // WORLD_HPP
