#include "Server.hpp"

void Server::commandPing(const Command &cmd, User &user) const
{
	if (cmd.args.empty()) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	const std::string &origin = cmd.args[0];
	if (origin.empty()) {
		this->errNoOrigin(user);
		return;
	}
	user.send(':' + m_hostname + " PONG " + origin + "\r\n");
}
