#include "Channel.hpp"
#include "Server.hpp"

void Server::commandJoin(const Command &command, User &user)
{
	if (command.args.size() < 1) {
		this->reply(ERR_NEEDMOREPARAMS, user);
		return;
	}
	const std::string &channel_name = command.args[0];
	std::vector<Channel *>::iterator channel_it = this->getChannelByName(channel_name);
	Channel *chan = NULL;
	if (channel_it == m_channels.end()) {
		chan = new Channel(channel_name, *this);
		m_channels.push_back(chan);
	}
	else {
		chan = (*channel_it);
	}
	this->connectUserToChannel(user, *chan);
	std::string msg = ":" + user.nickname + '!' + user.servername + "@localhost";
	msg.append(" JOIN " + chan->name() + "\r\n");
	chan->broadcast(msg);
	this->commandNames(Command("NAMES " + chan->name()), user);
}
