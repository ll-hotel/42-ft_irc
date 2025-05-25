#include "Server.hpp"

// implement me
static bool is_user_op(const User &user, const Channel &channel)
{
	return channel.ops.find(user.id) != channel.ops.end();
}

void Server::commandTopic(const Command &command, User &user)
{
	if (command.args.size() == 0) {
		this->reply(ERR_NEEDMOREPARAMS, user);
		return;
	}
	std::vector<Channel *>::iterator chan_it = this->getChannelByName(command.args[0]);

	std::string rply;
	if (chan_it == m_channels.end()) {
		rply = getReplyBase(ERR_NOTONCHANNEL, user) + " :";
		rply.append(ERR_NOTONCHANNEL_MESSAGE);
	}
	else if (command.args.size() == 2) {
		if (is_user_op(user, **chan_it)) {
			(*chan_it)->topic = command.args[1];
		}
		else {
			rply = getReplyBase(ERR_CHANOPRIVSNEEDED, user) + " :";
			rply.append(ERR_CHANOPRIVSNEEDED_MESSAGE);
		}
	}
	else {
		if ((*chan_it)->topic.size() == 0) {
			rply = getReplyBase(RPL_NOTOPIC, user) + " " + (*chan_it)->name() + " :";
			rply.append(RPL_NOTOPIC_MESSAGE);
		}
		else {
			rply = getReplyBase(RPL_TOPIC, user) + " " + (*chan_it)->name() + " :";
			rply.append((*chan_it)->topic);
		}
	}
	rply.append("\r\n");
	user.send(rply);
}
