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
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        switch (mType)
        {
            case 0: mWorld.createActor<Forest>(c.x,c.y); break;
            case 1: mWorld.createActor<GoldMine>(c.x,c.y); break;
            case 2: mWorld.createActor<Quarry>(c.x,c.y); break;
            case 3: mWorld.createActor<Hall>(c.x,c.y); break;
            case 4: mWorld.createActor<Market>(c.x,c.y); break;
            case 5: mWorld.createActor<Barrack>(c.x,c.y); break;
            default: break;
        }
    }

    // Right click
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
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
