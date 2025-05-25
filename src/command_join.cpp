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
	if (channel_it == m_channels.end()) {
		m_channels.push_back(new Channel(channel_name, *this));
		channel_it = m_channels.end() - 1;
		(*channel_it)->addOp(user.id);
	}
	Channel &channel = **channel_it;

	channel.addUser(user);
	std::string notification = ":" + user.nickname + '!' + user.servername + "@localhost" + ' ' +
							   "JOIN" + ' ' + channel.name() + "\r\n";
	channel.broadcast(notification);
	this->commandNames(channel, user);
}
