#include "../NodeEngine/Application/Application.hpp"

#include "States.hpp"

#include "Client.hpp"

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
    ah::Application::getStates().registerState<LoginState>();
    ah::Application::getStates().registerState<ListState>();
    ah::Application::getStates().pushState<LoginState>();

    int retry = 3;
    while (retry > 0)
    {
        std::cout << "Connecting..." << std::endl;
        if (Client::instance().connect(sf::IpAddress::LocalHost,4567,"Cmdu76","test"))
        {
            std::cout << "Connected !" << std::endl;
            ah::Application::run();
            retry = 0;
            std::cout << "Disconnected" << std::endl;
        }
        else
        {
            std::cout << "Connection failed" << std::endl;
            retry--;
        }
    }

    return 0;
}
