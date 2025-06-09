#include "Server.hpp"
#include "ft_split.hpp"
#include "utils.hpp"

void Server::commandPart(const Command &cmd, User &user)
{
	if (cmd.args.size() < 1) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	const std::string prefix = ':' + user.clientName(m_hostname) + " PART ";
	const std::string reason = (cmd.args.size() > 1 ? ' ' + cmd.args[1] : "");

	std::vector<std::string> chan_names = ft_split(cmd.args[0], ',');
	for (std::vector<std::string>::const_iterator chan_it = chan_names.begin();
		 chan_it != chan_names.end(); chan_it++) {
		std::string channel_name = strTolower(*chan_it);
		std::vector<Channel *>::iterator chan_pos = this->getChannelByName(channel_name);
		if (chan_pos == m_channels.end()) {
			this->errNoSuchChannel(user, channel_name);
			continue;
		}
		Channel &chan = *(*chan_pos);
		if (chan.users.find(user.id) == chan.users.end()) {
			this->errNotOnChannel(user, channel_name);
			continue;
		}
		const std::string msg(prefix + channel_name + reason + "\r\n");
		chan.broadcast(msg);
		this->removeChannelUser(chan, user);
	}
}
