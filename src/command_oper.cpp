#include "Server.hpp"

void Server::commandOper(const Command &command, User &user)
{
	if (command.args.size() < 2) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	if (command.args[1] != m_operatorPassWord) {
		this->errPasswdMismatch(user);
		return;
	}
	user.username = command.args[0];
	m_ops.insert(user.id);
	this->rplYoureOper(user);
}
