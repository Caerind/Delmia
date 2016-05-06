#include "World.hpp"

World::World()
{
    mMap = NWorld::createActor<Map>();
}

void World::handleEvent(sf::Event const& event)
{
    // Add event to the world system
    NWorld::addEvent(event);
}

void World::update(sf::Time dt)
{
    NWorld::tick(dt);

    sf::Vector2f m = NWorld::getPointerPositionView();
    m.x = (int)m.x;
    m.y = (int)m.y;
    sf::Vector2i t = NMapUtility::Isometric::worldToCoords(m);
    t.x = (int)t.x;
    t.y = (int)t.y;
    sf::Vector2i c = getMouseCoords();

    NWorld::getWindow().setDebugInfo("mouse",NString::toString(m));
    NWorld::getWindow().setDebugInfo("tile",NString::toString(t));
    NWorld::getWindow().setDebugInfo("collide",std::to_string(collide(t.x,t.y)));

    NWorld::getWindow().setDebugInfo("actors",std::to_string(NWorld::getActorCount()));
    NWorld::getWindow().setDebugInfo("tickables",std::to_string(NWorld::getTickableCount()));
    NWorld::getWindow().setDebugInfo("renderables",std::to_string(NWorld::getRenderableCount()));
    NWorld::getWindow().setDebugInfo("chunks",std::to_string(mMap->getChunkCount()));

    NWorld::getWindow().setDebugInfo("pos-g",std::to_string(c.x) + "," + std::to_string(c.y));
    NWorld::getWindow().setDebugInfo("pos-c",std::to_string(Map::globalToChunk(c).x) + "," + std::to_string(Map::globalToChunk(c).y));
    NWorld::getWindow().setDebugInfo("pos-r",std::to_string(Map::globalToRelative(c).x) + "," + std::to_string(Map::globalToRelative(c).y));
}

void World::render(sf::RenderTarget& target)
{
    NWorld::render();
}

sf::Vector2i World::getMouseCoords()
{
    return NMapUtility::Isometric::worldToCoords(NWorld::getPointerPositionView());
}

bool World::collide(int x, int y, bool isSolid)
{
    // Buildings
    for (auto itr = mBuildings.begin(); itr != mBuildings.end(); itr++)
    {
        if (itr->second->collide(x,y))
        {
            return true;
        }
    }

    if (isSolid)
    {
        // Resources
        for (auto itr = mResources.begin(); itr != mResources.end(); itr++)
        {
            if (itr->second->collide(x,y))
            {
                return true;
            }
        }

        // Map
        if (mMap->getTileId(x,y) == 3) // Water
        {
            return true;
        }
    }

    return false;
}

std::vector<Unit::Ptr> World::selectUnits(sf::FloatRect const& rect)
{
    std::vector<Unit::Ptr> units;
    for (auto itr = mUnits.begin(); itr != mUnits.end(); itr++)
    {
        if (rect.contains(itr->second->getPosition()))
        {
            units.push_back(itr->second);
        }
    }
    // TODO : Remove units from gamestate if deleted
    return units;
}

void World::clear()
{
    mMap = nullptr;
    NWorld::clear();
}
