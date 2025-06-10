#include "Server.hpp"

void Server::commandOper(const Command &command, User &user)
{
	if (command.args.size() < 2) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	if (command.args[1] != m_operatorPassWord) {
		errPasswMisMatch(user);
		return;
	}
	user.username = command.args[0];
	this->m_ops.insert(user.id);
	rplYourOper(user);
}
