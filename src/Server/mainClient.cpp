#include "Client.hpp"

int exampleMainClient()
{
    std::string username;
    std::string password;
    std::cout << "Username : ";
    std::cin >> username;
    std::cout << "Password : ";
    std::cin >> password;
    Client client;
    if (client.connect(sf::IpAddress::LocalHost,4567,username,password))
    {
        while (client.isConnected())
        {
            std::string line;
            std::getline(std::cin,line);
            client.handleCommand(line);
        }
    }

    return 0;
}
