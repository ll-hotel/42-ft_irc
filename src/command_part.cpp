#include "Server.hpp"
#include "ft_split.hpp"

void Server::commandPart(const Command &cmd, User &user)
{
	if (cmd.args.size() < 1) {
		this->reply(ERR_NEEDMOREPARAMS, user);
		return;
	}
	const std::string source(':' + user.nickname + '!' + user.username + '@' + m_hostname);
	const std::string action(" PART");
	const std::string prefix(source + action + ' ');
	const std::string reason(cmd.args.size() > 1 ? ' ' + cmd.args[1] : "");

	std::vector<std::string> chan_names = ft_split(cmd.args[0], ',');
	for (std::vector<std::string>::const_iterator chan_name = chan_names.begin();
		 chan_name != chan_names.end(); chan_name++) {
		std::vector<Channel *>::iterator chan_pos = this->getChannelByName(*chan_name);
		if (chan_pos == m_channels.end()) {
			this->errNoSuchChannel(user, *chan_name);
			continue;
		}
		Channel &chan = *(*chan_pos);
		if (chan.users.find(user.id) == chan.users.end()) {
			this->errNotOnChannel(user, *chan_name);
			continue;
		}
		const std::string msg(prefix + *chan_name + reason + "\r\n");
		chan.broadcast(msg);
		this->removeChannelUser(chan, user);
	}
}

void Server::errNoSuchChannel(User &user, const std::string &channel_name) const
{
	std::string msg(":" + m_hostname + ' ' + ft_ltoa(ERR_NOSUCHCHANNEL) + ' ');
	msg.append(user.nickname + "!" + user.username);
	msg.push_back(' ');
	msg.append(channel_name);
	msg.append(" :" ERR_NOSUCHCHANNEL_MESSAGE);
	msg.append("\r\n");
	user.send(msg);
}

void Server::errNotOnChannel(User &user, const std::string &channel_name) const
{
	std::string msg(":" + m_hostname + ' ' + ft_ltoa(ERR_NOSUCHCHANNEL) + ' ');
	msg.append(user.nickname + "!" + user.username);
	msg.push_back(' ');
	msg.append(channel_name);
	msg.append(" :" ERR_NOTONCHANNEL_MESSAGE);
	msg.append("\r\n");
	user.send(msg);
}
