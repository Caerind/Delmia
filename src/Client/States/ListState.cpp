#include "ListState.hpp"

ListState::ListState(ah::StateManager& manager)
: ah::State(manager)
{
}

bool ListState::handleEvent(sf::Event const& event)
{
    return true;
}

bool ListState::update(sf::Time dt)
{
    return true;
}

void ListState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}
