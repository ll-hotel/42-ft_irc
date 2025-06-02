#include "Server.hpp"

void Server::commandUser(const Command &command, User &user) const
{
	if (user.didUser) {
		this->errAlreadyRegistered(user);
		return;
	}
	if (command.args.size() != 4) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	user.username = command.args[0];
	user.hostname = command.args[1];
	user.servername = command.args[2];
	user.realname = command.args[3];
	user.didUser = true;
	if (user.registered())
		this->rplWelcome(user);
}
