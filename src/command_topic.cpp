#include "Server.hpp"
#include "utils.hpp"

void Server::commandTopic(const Command &command, User &user)
{
	if (command.args.size() == 0) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	std::string channel_name = strTolower(command.args[0]);
	std::vector<Channel *>::iterator chan_it = this->getChannelByName(channel_name);
	std::string rply;
	if (chan_it == m_channels.end()) {
		this->errNoSuchChannel(user, channel_name);
		return;
	}
	Channel &channel = *(*chan_it);
	if (user.channels.find(channel.id) == user.channels.end()) {
		this->errNotOnChannel(user, channel_name);
		return;
	}
	if (command.args.size() == 2) {
		if (!channel.topic_op_only or isUserOp(user, channel)) {
			channel.topic = command.args[1];
		}
		else {
			this->errChanOPrivsNeeded(user, channel.name());
			return;
		}
	}
	if (channel.topic.empty())
		this->rplNoTopic(user, channel.name());
	else
		this->rplTopic(user, channel.name(), channel.topic);
}
