#include "Server.hpp"

void Server::commandPass(const Command &command, User &user) const
{
	if (command.args.size() < 1) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	if (user.didPass) {
		this->errAlreadyRegistered(user);
		return;
	}
	if (command.args[0] != m_password)
		return;
	user.didPass = true;
}
