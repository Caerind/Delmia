#include "MainServer.hpp"

int main(int argc, char *argv[])
{
    MainServer::MainServer server;
    server.start();

    return EXIT_SUCCESS;
}
