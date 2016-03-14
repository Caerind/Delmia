#include "Game.hpp"

#include "NodeEngine/Core/World.hpp"

void registerGameClasses()
{
    NWorld::registerActor<Entity>();
    NWorld::registerActor<Building>();
    NWorld::registerActor<Unit>();
    NWorld::registerActor<Map>();
	
    NWorld::registerActor<Forest>();
}
