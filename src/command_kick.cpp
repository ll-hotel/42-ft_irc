#include "Server.hpp"
#include "ft_split.hpp"

void Server::commandKick(const Command &cmd, User &user) const
{
	if (cmd.args.size() < 2) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	const std::string &channel_name = cmd.args[0];
	std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(channel_name);
	if (chan_pos == m_channels.end()) {
		this->errNoSuchChannel(user, channel_name);
		return;
	}
	Channel &channel = *(*chan_pos);
	if (channel.ops.find(user.id) == channel.ops.end()) {
		this->errChanOPrivsNeeded(user, channel_name);
		return;
	}
	if (channel.users.find(user.id) == channel.users.end()) {
		this->errNotOnChannel(user, channel_name);
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
		User &channel_user = *(*chan_user_pos);
		if (channel.users.find(channel_user.id) == channel.users.end()) {
			this->errUserNotInChannel(user, channel_user.nickname, channel_name);
			continue;
		}
		channel.broadcast(':' + user.clientName(m_hostname) + " KICK " + channel_name + ' ' +
						  channel_user.nickname + " :" + message + "\r\n");
		channel.users.erase(channel_user.id);
		channel_user.channels.erase(channel.id);
	}
}
