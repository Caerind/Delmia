#include "World.hpp"

World::World()
{
    NWorld::registerActor<Entity>();
    NWorld::registerActor<Building>();
    NWorld::registerActor<Unit>();
    NWorld::registerActor<Map>();
    NWorld::registerActor<Forest>();
    //NWorld::registerActor<Hall>();

    mMap = NWorld::createActor<Map>();
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
    NWorld::update();

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
}

void World::render(sf::RenderTarget& target)
{
    NWorld::render(target);

    NWorld::getWindow().setView(NWorld::getCameraManager().getActiveView());

    for (auto itr = mBuildings.begin(); itr != mBuildings.end(); itr++)
    {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::Red);
        shape.setOutlineThickness(1.f);
        sf::FloatRect r = itr->second->getBounds();
        shape.setSize(sf::Vector2f(r.width,r.height));
        shape.setPosition(sf::Vector2f(r.left,r.top));
        target.draw(shape);
    }
}

sf::Vector2i World::getMouseCoords()
{
    return NMapUtility::Isometric::worldToCoords(NWorld::getPointerPositionView());
}

bool World::collide(int x, int y)
{
    // Buildings
    for (auto itr = mBuildings.begin(); itr != mBuildings.end(); itr++)
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

    return false;
}
