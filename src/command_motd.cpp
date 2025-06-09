#include "Server.hpp"

void Server::commandMOTD(const Command &cmd, User &user) const
{
	if (cmd.args.size() > 0) {
		if (cmd.args[0] != m_hostname) {
			this->errNoSuchServer(user, cmd.args[0]);
			return;
		}
	}
	if (m_motd.empty()) {
		this->errNoMOTD(user);
		return;
	}
	this->rplMOTDStart(user);
	this->rplMOTD(user);
	this->rplEndOfMOTD(user);
}
