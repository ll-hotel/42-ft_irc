#include "Server.hpp"
#include "utils.hpp"

void Server::commandNick(const Command &command, User &user) const
{
	if (command.args.size() < 1) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	const std::string &new_nick = command.args[0];
	if (not isvalid_nick(new_nick)) {
		this->errErroneusNickname(user, new_nick);
		return;
	}
	if (user.nickname == new_nick) {
		this->errNicknameInUse(user, new_nick);
		return;
	}
	for (size_t i = 0; i < m_users.size(); i += 1) {
		if (m_users[i]->nickname == new_nick) {
			this->errNickCollision(user, new_nick);
			return;
		}
	}
	user.nickname = new_nick;
	user.didNick = true;
}
