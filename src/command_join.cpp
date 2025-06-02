#include "Channel.hpp"
#include "Server.hpp"
#include <cctype>

static bool is_valid_channel_name(const std::string &name) throw();

void Server::commandJoin(const Command &command, User &user)
{
	if (command.args.size() < 1) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	const std::string &channel_name = command.args[0];
	if (not is_valid_channel_name(channel_name)) {
		this->errNoSuchChannel(user, channel_name);
		return;
	}
	std::vector<Channel *>::iterator channel_it = this->getChannelByName(channel_name);
	Channel *chan = NULL;
	if (channel_it == m_channels.end()) {
		chan = new Channel(channel_name, *this);
		m_channels.push_back(chan);
	}
	else {
		chan = (*channel_it);
	}
	if (chan->password_set) {
		if (command.args.size() < 2 or command.args[1] != chan->password) {
			this->errBadChannelKey(user, chan->name());
			return;
		}
	}
	if (chan->users.size() >= chan->limit_user) {
		this->errChannelIsFull(user, chan->name());
		return;
	}
	this->connectUserToChannel(user, *chan);
	chan->broadcast(':' + user.clientName(m_hostname) + " JOIN " + chan->name() + "\r\n");
}

static inline bool is_valid_channel_char(const char &c) throw()
{
	return std::isprint(c) and c != ' ' and c != ',';
}

static bool is_valid_channel_name(const std::string &name) throw()
{
	if (name.size() < 2 or name.size() >= 50) {
		return false;
	}
	const char &prefix = name[0];
	if (prefix != '#' and prefix != '&') {
		return false;
	}
	for (size_t i = 1; i < name.size(); i += 1) {
		if (not is_valid_channel_char(name[i])) {
			return false;
		}
	}
	return true;
}
