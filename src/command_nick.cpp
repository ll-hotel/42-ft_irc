#include "Server.hpp"

void Server::commandNick(const Command &command, User &user) const
{
	if (command.args.size() < 1) {
		this->reply(ERR_NONICKNAMEGIVEN, user);
		return;
	}
	const std::string &new_nick = command.args[0];
	if (user.nickname == new_nick) {
		this->reply(ERR_NICKNAMEINUSE, user);
		return;
	}
	for (size_t i = 0; i < m_users.size(); i += 1) {
		if (m_users[i]->nickname == new_nick) {
			this->reply(ERR_NICKCOLLISION, user);
			return;
		}
	}
	user.nickname = new_nick;
	user.didNick = true;
	if (user.registered())
		this->reply(RPL_WELCOME, user);
}
