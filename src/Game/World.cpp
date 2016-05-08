#include "World.hpp"

World::World()
{
    mMap = NWorld::createActor<Map>();

    NIsometric::setLayerSize(sf::Vector2i(16,64));
    NIsometric::setTileSize(sf::Vector2i(256,128));
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
    sf::Vector2i t = NIsometric::worldToCoords(m);
    sf::Vector2i c = NIsometric::worldToChunk(m);
    sf::Vector2i r = NIsometric::worldToRelative(m);
    sf::Vector2i mc = getMouseCoords();

    NWorld::getWindow().setDebugInfo("mouse",NString::toString(m));
    NWorld::getWindow().setDebugInfo("tile",NString::toString(t));
    NWorld::getWindow().setDebugInfo("pos-g",NString::toString(mc));
    NWorld::getWindow().setDebugInfo("pos-c",NString::toString(c));
    NWorld::getWindow().setDebugInfo("pos-r",NString::toString(r));
    NWorld::getWindow().setDebugInfo("collide",std::to_string(collide(t)));
    NWorld::getWindow().setDebugInfo("tile-id",std::to_string(mMap->getTileId(c,r)));

    NWorld::getWindow().setDebugInfo("actors",std::to_string(NWorld::getActorCount()));
    NWorld::getWindow().setDebugInfo("tickables",std::to_string(NWorld::getTickableCount()));
    NWorld::getWindow().setDebugInfo("renderables",std::to_string(NWorld::getRenderableCount()));
    NWorld::getWindow().setDebugInfo("chunks",std::to_string(mMap->getChunkCount()));

}

void World::render(sf::RenderTarget& target)
{
    NWorld::render();
}

sf::Vector2i World::getMouseCoords()
{
    return NIsometric::worldToCoords(NWorld::getPointerPositionView());
}

bool World::collide(sf::Vector2i const& coords, bool isSolid)
{
    // Buildings
    for (auto itr = mBuildings.begin(); itr != mBuildings.end(); itr++)
    {
        if (itr->second->collide(coords.x,coords.y))
        {
            return true;
        }
    }

    if (isSolid)
    {
        // Resources
        for (auto itr = mResources.begin(); itr != mResources.end(); itr++)
        {
            if (itr->second->collide(coords.x,coords.y))
            {
                return true;
            }
        }

        // Map
        if (mMap->getTileId(coords) == 3) // Water
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
    return units;
}

void World::removeActor(std::string const& id)
{
    NWorld::removeActor(id);

    if (contains(mBuildings, id))
    {
        remove(mBuildings, id);
    }
    if (contains(mResources, id))
    {
        remove(mResources, id);
    }
    if (contains(mUnits,id))
    {
        remove(mUnits, id);
        // TODO : Remove selected units from game state
    }
}

void World::clear()
{
    mMap = nullptr;
    NWorld::clear();
}

std::shared_ptr<Map> World::getMap()
{
    return mMap;
}
