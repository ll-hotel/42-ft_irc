#include "Server.hpp"

void Server::commandPrivMsg(const Command &cmd, User &user) const
{
	if (cmd.args.size() < 1) {
		this->errNoRecipient(user, "PRIVMSG");
		return;
	}
	const std::string &dest = cmd.args[0];
	if (cmd.args.size() < 2) {
		this->errNoTextToSend(user);
		return;
	}
	const std::string &message = cmd.args[1];
	{
		std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(dest);
		if (chan_pos != m_channels.end()) {
			Channel &dest_chan = *(*chan_pos);
			std::string response = ":" + user.nickname + '!' + user.username + '@' + m_hostname +
								   " PRIVMSG " + dest_chan.name() + " :" + message + "\r\n";
			dest_chan.privmsg(user.id, response);
			return;
		}
	}
	{
		std::vector<User *>::const_iterator user_pos = this->getUserbyNickname(dest);
		if (user_pos != m_users.end()) {
			User &dest_user = *(*user_pos);
			dest_user.send(":" + user.nickname + '!' + user.username + '@' + m_hostname +
						   " PRIVMSG " + dest_user.nickname + " :" + message + "\r\n");
			return;
		}
	}
	this->errNoSuchNick(user, dest);
}

void Server::errNoRecipient(User &user, const std::string &cmd) const
{
	user.send(":" + m_hostname + ' ' + ft_ltoa(ERR_NORECIPIENT) + ' ' + user.nickname +
			  " :" ERR_NORECIPIENT_MESSAGE ": " + cmd + "\r\n");
}

void Server::errNoTextToSend(User &user) const
{
	user.send(":" + m_hostname + ' ' + ft_ltoa(ERR_NORECIPIENT) + ' ' + user.nickname +
			  " :" ERR_NORECIPIENT_MESSAGE "\r\n");
}

void Server::errNoSuchNick(User &user, const std::string &nick) const
{
	user.send(":" + m_hostname + ' ' + ft_ltoa(ERR_NOSUCHNICK) + ' ' + user.nickname + ' ' + nick +
			  " :" ERR_NOSUCHNICK_MESSAGE "\r\n");
}
