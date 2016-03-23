#include "Server.hpp"

int exampleMainServer()
{
    Server server;
    server.start();

    while (server.isRunning())
    {
        std::string line;
        std::getline(std::cin,line);
        server.handleCommand(line);
    }

    return 0;
}
