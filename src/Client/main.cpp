#include "../NodeEngine/Application/Application.hpp"

#include "States.hpp"

int main()
{
    ah::Application::getResources().loadTexture("iso","assets/textures/iso.png");
    ah::Application::getResources().loadTexture("building","assets/textures/building.png");
    ah::Application::getResources().loadTexture("unit","assets/textures/unit.png");
    ah::Application::getResources().loadFont("coolveticca","assets/fonts/coolveticca.ttf");

    ah::Application::getWindow().create(sf::VideoMode(800,600),"NodeEngine",sf::Style::Close);
    ah::Application::getWindow().showDebugInfo(true);
    ah::Application::getWindow().setDebugInfoFont(&ah::Application::getResources().getFont("coolveticca"));
    ah::Application::getWindow().setDebugInfoColor(sf::Color::White);

    ah::Application::getStates().registerState<GameState>();
    ah::Application::getStates().pushState<GameState>();

    ah::Application::run();

    return 0;
}
