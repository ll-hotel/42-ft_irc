#include "Server.hpp"
// #include <iostream>

void Server::commandQuit(const Command &command, User &user)
{
	if (command.args.size() == 1)
		user.quitMessage = command.args[0];
	user.quit = true;
}
