#include "Server.hpp"
#include <iostream>

void Server::commandQuit(const Command &command, User &user)
{
	if (command.args.size() == 1)
		user.quitMessage = command.args[0];
	std::cerr << user.nickname << ": " << user.quitMessage << std::endl;
	user.quit = true;
}
