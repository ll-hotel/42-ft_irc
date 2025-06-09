#include "Server.hpp"
#include "utils.hpp"

void Server::commandPrivMsg(const Command &cmd, User &user) const
{
	if (cmd.args.size() < 1) {
		this->errNoRecipient(user, cmd.name);
		return;
	}
	const std::string dest = strTolower(cmd.args[0]);
	if (cmd.args.size() < 2) {
		this->errNoTextToSend(user);
		return;
	}
	const std::string &message = cmd.args[1];
	{
		std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(dest);
		if (chan_pos != m_channels.end()) {
			Channel &dest_chan = *(*chan_pos);
			dest_chan.privmsg(user.id, ':' + user.clientName(m_hostname) + " PRIVMSG " +
										   dest_chan.name() + " :" + message + "\r\n");
			return;
		}
	}
	{
		std::vector<User *>::const_iterator user_pos = this->getUserbyNickname(dest);
		if (user_pos != m_users.end()) {
			User &dest_user = *(*user_pos);
			dest_user.send(':' + user.clientName(m_hostname) + " PRIVMSG " + dest_user.nickname +
						   " :" + message + "\r\n");
			return;
		}
	}
	this->errNoSuchNick(user, dest);
}
