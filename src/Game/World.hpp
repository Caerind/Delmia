#ifndef WORLD_HPP
#define WORLD_HPP

#include "../NodeEngine/Core/World.hpp"
#include "../NodeEngine/Core/MapUtility.hpp"

#include "../Game.hpp"

class World
{
    public:
        World();

        void handleEvent(sf::Event const& event);
        void update(sf::Time dt);
        void render(sf::RenderTarget& target);

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
    std::shared_ptr<T> actor = NWorld::createActor<T>(std::forward<Args>(args)...);

    std::shared_ptr<Building> building;
    if(building = std::dynamic_pointer_cast<Building>(actor))
    {
        mBuildings[actor->getId()] = building;
        NLog::log("Building added");
    }

    std::shared_ptr<Unit> unit;
    if(unit = std::dynamic_pointer_cast<Unit>(actor))
    {
        mUnits[actor->getId()] = unit;
        NLog::log("Unit added");
    }
    return actor;
}

#endif // WORLD_HPP
