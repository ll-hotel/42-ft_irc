#include "Server.hpp"
#include "ft_split.hpp"

void Server::commandKick(const Command &cmd, User &user) const
{
	if (cmd.args.size() < 2) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	const std::string &chan_name = cmd.args[0];
	std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(chan_name);
	if (chan_pos == m_channels.end()) {
		this->errNoSuchChannel(user, chan_name);
		return;
	}
	Channel &chan = *(*chan_pos);
	if (chan.ops.find(user.id) == chan.ops.end()) {
		this->errChanOPrivsNeeded(user, chan_name);
		return;
	}
	if (chan.users.find(user.id) == chan.users.end()) {
		this->errNotOnChannel(user, chan_name);
		return;
	}
	const std::string message = (cmd.args.size() >= 3) ? cmd.args[2] : "Kicked by an operator";
	std::vector<std::string> nicknames = ft_split(cmd.args[1], ',');
	for (size_t i = 0; i < nicknames.size(); i += 1) {
		const std::string &nick = nicknames[i];
		std::vector<User *>::const_iterator chan_user_pos = this->getUserbyNickname(nick);
		if (chan_user_pos == m_users.end()) {
			this->errNoSuchNick(user, nick);
			continue;
		}
		User &chan_user = *(*chan_user_pos);
		if (chan.users.find(chan_user.id) == chan.users.end()) {
			this->errUserNotInChannel(user, chan_user.nickname, chan_name);
			continue;
		}
		chan.broadcast(':' + user.nickname + '!' + user.username + '@' + m_hostname + " KICK " +
					   chan_name + ' ' + chan_user.nickname + " :" + message + "\r\n");
		chan.users.erase(chan_user.id);
		chan_user.channels.erase(chan.id);
	}
}

void Server::errChanOPrivsNeeded(User &user, const std::string &channel_name) const
{
	user.send(':' + m_hostname + ' ' + ft_ltoa(ERR_CHANOPRIVSNEEDED) + ' ' + user.nickname + ' ' +
			  channel_name + " :" ERR_CHANOPRIVSNEEDED_MESSAGE "\r\n");
}

void Server::errUserNotInChannel(User &user, const std::string &nick,
								 const std::string &channel) const
{
	user.send(':' + m_hostname + ' ' + ft_ltoa(ERR_USERNOTINCHANNEL) + ' ' + user.nickname + ' ' +
			  nick + ' ' + channel + " :" ERR_USERNOTINCHANNEL_MESSAGE "\r\n");
}
