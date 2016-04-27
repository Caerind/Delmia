#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
    mType = 0;
}

bool GameState::handleEvent(sf::Event const& event)
{
    mWorld.handleEvent(event);

    // Coordonnées
    sf::Vector2i c = mWorld.getMouseCoords();

    // Left Click
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        switch (mType)
        {
            case 0: mWorld.createBuilding<Forest>(c.x,c.y); break;
            case 1: mWorld.createBuilding<GoldMine>(c.x,c.y); break;
            case 2: mWorld.createBuilding<Quarry>(c.x,c.y); break;
            case 3: mWorld.createBuilding<Hall>(c.x,c.y); break;
            case 4: mWorld.createBuilding<Market>(c.x,c.y); break;
            case 5: mWorld.createBuilding<Barrack>(c.x,c.y); break;
            default: break;
        }
    }

    // Right click
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
    {
        mType = (mType >= 6)? 0 : mType+1;
    }

    return true;
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    return true;
}

void GameState::render(sf::RenderTarget& target, sf::RenderStates states)
{
    mWorld.render(target);
}
