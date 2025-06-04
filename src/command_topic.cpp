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
		this->errNotOnChannel(user, channel_name);
		return;
	}
	else if (command.args.size() == 2) {
		if (!(**chan_it).topic_op_only or isUserOp(user, **chan_it)) {
			(*chan_it)->topic = command.args[1];
		}
		else {
			this->errChanOPrivsNeeded(user, (*chan_it)->name());
			return;
		}
	}

	if ((*chan_it)->topic.size() == 0) {
		rply = getReplyBase(RPL_NOTOPIC, user) + " " + (*chan_it)->name() + " :";
		rply.append(RPL_NOTOPIC_MESSAGE);
		user.send(rply + "\r\n");
		return;
	}
	else {
		rply = getReplyBase(RPL_TOPIC, user) + " " + (*chan_it)->name() + " :";
		rply.append((*chan_it)->topic);
		user.send(rply + "\r\n");
		return;
	}
}
