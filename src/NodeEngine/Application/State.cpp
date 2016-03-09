#include "State.hpp"
#include "StateManager.hpp"

namespace ah
{

State::State(StateManager& manager) : mManager(manager)
{
}

State::~State()
{
}

bool State::handleEvent(sf::Event const& event)
{
    return true;
}

bool State::update(sf::Time dt)
{
    return true;
}

void State::render(sf::RenderTarget& target, sf::RenderStates states)
{
}

void State::onActivate()
{
}

void State::onDeactivate()
{
}

void State::requestPush(std::string const& id)
{
    mManager.pushState(id);
}

void State::requestPop()
{
    mManager.popState();
}

void State::requestClear()
{
    mManager.clearStates();
}

} // namespace ah
