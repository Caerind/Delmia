#include "MainServer.hpp"

int main(int argc, char *argv[])
{
    MainServer::MainServer server;
    server.start();

    while (server.isRunning())
    {
        std::string line;
        std::getline(std::cin,line);
        server.handleCommand(line);
    }

    return EXIT_SUCCESS;
}
