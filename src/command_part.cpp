#include "Server.hpp"

static std::vector<std::string> ft_split(const std::string &str, const char sep);

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
			this->errNoSuchChannel(*chan_name, user);
			continue;
		}
		Channel &chan = *(*chan_pos);
		if (chan.users.find(user.id) == chan.users.end()) {
			this->errNotOnChannel(*chan_name, user);
			continue;
		}
		const std::string msg(prefix + *chan_name + reason + "\r\n");
		chan.broadcast(msg);
		this->removeChannelUser(chan, user);
	}
}

void Server::errNoSuchChannel(const std::string &chan_name, User &user)
{
	std::string msg(":" + m_hostname + ' ' + ft_ltoa(ERR_NOSUCHCHANNEL) + ' ');
	msg.append(user.nickname + "!" + user.username);
	msg.push_back(' ');
	msg.append(chan_name);
	msg.append(" :" ERR_NOSUCHCHANNEL_MESSAGE);
	msg.append("\r\n");
	user.send(msg);
}

void Server::errNotOnChannel(const std::string &chan_name, User &user)
{
	std::string msg(":" + m_hostname + ' ' + ft_ltoa(ERR_NOSUCHCHANNEL) + ' ');
	msg.append(user.nickname + "!" + user.username);
	msg.push_back(' ');
	msg.append(chan_name);
	msg.append(" :" ERR_NOTONCHANNEL_MESSAGE);
	msg.append("\r\n");
	user.send(msg);
}

static std::vector<std::string> ft_split(const std::string &str, const char sep)
{
	std::vector<std::string> pieces;
	size_t pos = 0;
	size_t piece_end = 0;
	for (pos = 0; pos < str.size(); pos = piece_end) {
		while (pos < str.size() and str[pos] == sep)
			pos += 1;
		if (pos == str.size())
			break;
		const size_t piece_pos = pos;
		while (pos < str.size() and str[pos] != sep)
			pos += 1;
		piece_end = pos;
		pieces.push_back(str.substr(piece_pos, piece_end - piece_pos));
	}
	return pieces;
}
