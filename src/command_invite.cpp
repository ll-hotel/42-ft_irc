#include "Server.hpp"

void Server::commandInvite(const Command &cmd, User &user) const
{
	if (cmd.args.size() < 2) {
		this->errNeedMoreParams(user, "INVITE");
		return;
	}
	const std::string &channel_name = cmd.args[1];
	std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(channel_name);
	if (chan_pos == m_channels.end()) {
		this->errNoSuchChannel(user, channel_name);
		return;
	}
	Channel &channel = *(*chan_pos);
	if (channel.users.find(user.id) == channel.users.end()) {
		this->errNotOnChannel(user, channel_name);
		return;
	}
	if (channel.inviteOnly and channel.ops.find(user.id) == channel.ops.end()) {
		this->errChanOPrivsNeeded(user, channel_name);
		return;
	}
	const std::string &nickname = cmd.args[0];
	std::vector<User *>::const_iterator nick_user_pos = this->getUserbyNickname(nickname);
	if (nick_user_pos == m_users.end()) {
		this->errNoSuchNick(user, nickname);
		return;
	}
	User &nick_user = *(*nick_user_pos);
	if (channel.inviteList.find(nick_user.id) == channel.inviteList.end()) {
		channel.inviteList.insert(nick_user.id);
	}
	this->rplInviting(user, nickname, channel_name);
	const std::string client = user.nickname + '!' + user.username + '@' + m_hostname;
	nick_user.send(':' + client + " INVITE " + nickname + ' ' + channel_name + "\r\n");
}

void Server::rplInviting(User &user, const std::string &nick, const std::string &channel) const
{
	const std::string client = user.nickname + '!' + user.username + '@' + m_hostname;
	user.send(':' + m_hostname + ' ' + ft_ltoa(RPL_INVITING) + ' ' + client + ' ' + nick + ' ' +
			  channel + "\r\n");
}
