#include "GameServer.hpp"

int main(int argc, char *argv[])
{
    GameServer server;
	server.start();
	
	while (server.isRunning())
	{
		std::string command;
		std::getline(std::cin, command);
		server.handleCommand(command);
	}
	
    return EXIT_SUCCESS;
}




