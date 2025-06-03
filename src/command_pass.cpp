#include "Server.hpp"
#include <iostream>

void Server::commandPass(const Command &command, User &user) const
{
	if (command.args.size() < 1) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	if (command.args[0] != m_password)
		return;
	if (user.didPass) {
		this->errAlreadyRegistered(user);
		return;
	}
	user.didPass = true;
	std::cerr << "User " << user.stream.rawFd() << " authenticated" << std::endl;
}
