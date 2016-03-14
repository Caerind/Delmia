#include "States.hpp"

#include "../NodeEngine/Application/Application.hpp"

void registerStates()
{
    ah::Application::getStates().registerState<GameState>();
}
