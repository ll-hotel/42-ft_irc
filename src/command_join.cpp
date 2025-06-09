#include "Channel.hpp"
#include "Server.hpp"
#include "ft_split.hpp"
#include "utils.hpp"

static bool is_valid_channel_name(const std::string &name) throw();

void Server::commandJoin(const Command &cmd, User &user)
{
	if (cmd.args.size() < 1) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	std::vector<std::string> channel_names = ft_split(cmd.args[0], ',');
	std::vector<std::string> channel_keys;
	if (cmd.args.size() >= 2) {
		channel_keys = ft_split(cmd.args[1], ',');
	}
	for (size_t channel_i = 0; channel_i < channel_names.size(); channel_i += 1) {
		const std::string channel_name = strTolower(channel_names[channel_i]);
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
		if (this->m_ops.find(user.id) == this->m_ops.end()) {
			if (chan->password_set) {
				if (channel_keys.size() <= channel_i or channel_keys[channel_i] != chan->password) {
					this->errBadChannelKey(user, chan->name());
					continue;
				}
			}
			if (chan->users.size() >= chan->limit_user) {
				this->errChannelIsFull(user, chan->name());
				return;
			}
			if (chan->invite_only && user.inviteList.find(chan->id) == user.inviteList.end()) {
				errInviteOnlyChan(user, chan->name());
				return;
			}
		}
		this->connectUserToChannel(user, *chan);
		chan->broadcast(':' + user.clientName(m_hostname) + " JOIN " + chan->name() + "\r\n");
	}
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
