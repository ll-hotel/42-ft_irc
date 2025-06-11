#include "Server.hpp"
#include "ft_split.hpp"
#include "utils.hpp"

void Server::commandPrivMsg(const Command &cmd, User &user) const
{
	if (cmd.args.size() < 1) {
		this->errNoRecipient(user, cmd.name);
		return;
	}
	const std::string targets_str = strTolower(cmd.args[0]);
	if (cmd.args.size() < 2) {
		this->errNoTextToSend(user);
		return;
	}
	const std::vector<std::string> targets = ft_split(targets_str, ',');
	const std::string &message = cmd.args[1];
	for (std::vector<std::string>::const_iterator target_it = targets.begin();
		 target_it != targets.end(); target_it++) {
		const std::string &target = *target_it;
		{
			std::vector<Channel *>::const_iterator chan_pos = this->getChannelByName(target);
			if (chan_pos != m_channels.end()) {
				Channel &dest_chan = *(*chan_pos);
				if (dest_chan.users.find(user.id) != dest_chan.users.end()) {
					this->errNotOnChannel(user, dest_chan.name());
				}
				else {
					dest_chan.privmsg(user.id, ':' + user.clientName(m_hostname) + " PRIVMSG " +
												   dest_chan.name() + " :" + message + "\r\n");
				}
				continue;
			}
		}
		{
			std::vector<User *>::const_iterator user_pos = this->getUserbyNickname(target);
			if (user_pos != m_users.end()) {
				User &dest_user = *(*user_pos);
				dest_user.send(':' + user.clientName(m_hostname) + " PRIVMSG " +
							   dest_user.nickname + " :" + message + "\r\n");
				continue;
			}
		}
		this->errNoSuchNick(user, targets_str);
	}
}
