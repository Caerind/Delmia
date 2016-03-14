#include "MainServer.hpp"

int main(int argc, char *argv[])
{
    MainServer server;
    server.start();

    while (server.isRunning())
    {
        std::string command;
        std::getline(std::cin, command);
        server.handleCommand(command);
    }

    return EXIT_SUCCESS;
}
