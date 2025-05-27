#include "Channel.hpp"
#include "Server.hpp"
#include "ft_split.hpp"

void Server::commandNames(const Command &cmd, User &user) const
{
	if (cmd.args.empty()) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	std::vector<std::string> chan_names = ft_split(cmd.args[0], ',');
	for (size_t i = 0; i < chan_names.size(); i += 1) {
		this->rplNamReply(user, chan_names[i]);
	}
}

void Server::rplNamReply(User &user, const std::string &chan_name) const
{
	std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(chan_name);
	if (chan_pos == m_channels.end()) {
		this->rplEndOfNames(user, chan_name);
		return;
	}
	Channel &chan = *(*chan_pos);
	const std::string prefix = ':' + m_hostname + ' ' + ft_ltoa(RPL_NAMREPLY) + ' ' = user.nickname;

	const char chan_perm = '=';

	std::string nickname_list;
	for (size_t i = 0; i < chan.users.size(); i += 1) {
		nickname_list.push_back(' ');
		nickname_list.append(m_users[i]->nickname);
	}
	if (nickname_list.empty()) {
		nickname_list.push_back(':');
	} else {
		((char *)nickname_list.data())[0] = ':';
	}
	user.send(prefix + ' ' + chan_perm + ' ' + chan.name() + ' ' + nickname_list + "\r\n");
}

void Server::errNeedMoreParams(User &user, const std::string &cmd) const
{
	user.send(':' + m_hostname + ' ' + ft_ltoa(ERR_NEEDMOREPARAMS) + ' ' + user.nickname + ' ' +
			  cmd + " :" ERR_NEEDMOREPARAMS_MESSAGE "\r\n");
}

void Server::rplEndOfNames(User &user, const std::string &chan) const
{
	user.send(':' + m_hostname + ' ' + ft_ltoa(RPL_ENDOFNAMES) + ' ' + user.nickname + ' ' + chan +
			  " :" RPL_ENDOFNAMES_MESSAGE "\r\n");
}
