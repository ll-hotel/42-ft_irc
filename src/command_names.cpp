#include "Channel.hpp"
#include "Server.hpp"

void Server::commandNames(const Channel &chan, User &user) const
{
	std::string msg;

	msg.push_back(':');
	msg.append(m_hostname);
	msg.push_back(' ');
	msg.append(ft_ltoa(RPL_NAMREPLY));
	msg.append(" = ");
	msg.append(chan.name());
	msg.append(" :");
	for (size_t i = 0; i < chan.users.size(); i += 1) {
		msg.append(m_users[i]->nickname);
		msg.push_back(' ');
	}
	msg.append("\r\n");
	user.send(msg);
}
