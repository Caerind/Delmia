#include "World.hpp"

World::World()
{
    mMap = NWorld::createActor<Map>();
    mMap->addChunk(0,0);
    mMap->addChunk(0,1);
    mMap->addChunk(0,2);
    mMap->addChunk(0,3);
    mMap->addChunk(0,4);
    mMap->addChunk(1,0);
    mMap->addChunk(2,0);
    mMap->addChunk(3,0);
    mMap->addChunk(2,2);
    mMap->addChunk(-1,-1);
    mMap->addChunk(-2,-2);
}

void World::handleEvent(sf::Event const& event)
{
    // Add event to the world system
    NWorld::addEvent(event);

    // Zoom
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
        if (event.mouseWheelScroll.delta < 1)
        {
            NWorld::getCameraManager().getView().zoom(1.2f);
        }
        else
        {
            NWorld::getCameraManager().getView().zoom(0.8f);
        }
    }
}

void World::update(sf::Time dt)
{
    NWorld::tick(dt);

    sf::Vector2i c = getMouseCoords();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
    {
        mMap->setTileId(c.x,c.y,3);
    }

    sf::Vector2f mvt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        mvt.y--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        mvt.x--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        mvt.y++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        mvt.x++;
    }
    NWorld::getCameraManager().getView().move(dt.asSeconds() * 400.f * mvt);

    NVector m = NWorld::getPointerPositionView();
    m.x = (int)m.x;
    m.y = (int)m.y;
    NVector t = NVector::SFML2IToN(NMapUtility::Isometric::worldToCoords(m));
    t.x = (int)t.x;
    t.y = (int)t.y;

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

void World::clear()
{
    mMap = nullptr;
    NWorld::clear();
}
