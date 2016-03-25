#include "GameState.hpp"

GameState::GameState(ah::StateManager& manager)
: ah::State(manager)
{
}

bool GameState::handleEvent(sf::Event const& event)
{
    mWorld.handleEvent(event);
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
