#include "LoginState.hpp"

LoginState::LoginState(ah::StateManager& manager)
: ah::State(manager)
{
}

bool LoginState::handleEvent(sf::Event const& event)
{
    return true;
}

bool LoginState::update(sf::Time dt)
{
    return true;
}

void LoginState::render(sf::RenderTarget& target, sf::RenderStates states)
{
}
