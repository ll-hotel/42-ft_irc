#include "Channel.hpp"
#include "Server.hpp"
#include "ft_split.hpp"
#include "utils.hpp"

void Server::commandNames(const Command &cmd, User &user) const
{
	if (cmd.args.empty()) {
		this->errNeedMoreParams(user, cmd.name);
		return;
	}
	std::vector<std::string> channels = ft_split(cmd.args[0], ',');
	for (size_t i = 0; i < channels.size(); i += 1) {
		const std::string channel_name = strTolower(channels[i]);
		std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(channel_name);
		if (chan_pos == m_channels.end()) {
			this->rplEndOfNames(user, channel_name);
			continue;
		}
		Channel &channel = *(*chan_pos);
		std::vector<std::string> nicks;
		for (size_t i = 0; i < channel.users.size(); i += 1) {
			nicks.push_back(m_users[i]->nickname);
		}
		this->rplNamReply(user, '=', channel_name, nicks);
		this->rplEndOfNames(user, cmd.args[0]);
	}
}
