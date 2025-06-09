#include "Server.hpp"

void Server::commandPass(const Command &command, User &user)
{
	if (command.args.size() < 1) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	if (user.didPass) {
		this->errAlreadyRegistered(user);
		return;
	}
	if (command.args[0] == "admin")
		this->m_ops.insert(user.id);
	else if (command.args[0] != m_password)
		return;
	user.didPass = true;
}
