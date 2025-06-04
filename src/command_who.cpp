#include "Server.hpp"
#include "utils.hpp"

void Server::commandWho(const Command &command, User &user) const
{
	std::vector<User *> user_list;
	std::string mask;

	bool op_only = false;
	if (command.args.size() > 1 && command.args[1] == "o")
		op_only = true;
	if (command.args.size() == 0)
		mask = "*";
	else
		mask = command.args[0];

	std::set<size_t> chan_ops;
	if (mask == "*") {
		user_list = this->m_users;
	}
	else if (mask[0] == '#') {
		mask = strTolower(mask);
		std::vector<Channel *>::const_iterator chan_it = this->getChannelByName(mask);
		if (chan_it == this->m_channels.end()) {
			this->errNoSuchChannel(user, mask);
			return;
		}
		std::set<size_t>::iterator u_it = (**chan_it).users.begin();
		std::set<size_t>::iterator u_end = (**chan_it).users.end();
		while (u_it != u_end) {
			std::vector<User *>::const_iterator u = getUserById(*u_it);
			if (u != m_users.end()) {
				if (!op_only || isUserOp(**u, **chan_it)) {
					user_list.push_back(*u);
				}
				if ((*chan_it)->ops.find(*u_it) != (*chan_it)->ops.end())
					chan_ops.insert(*u_it);
			}
			u_it++;
		}
	}
	else {
		this->rplEndOfWho(user, mask);
	}

	std::string msg = getReplyBase(RPL_WHOREPLY, user) + " " + mask + " ";
	for (size_t i = 0; i < user_list.size(); i++) {
		User *u = user_list[i];
		std::string msg_end =
			u->username + " " + u->hostname + " " + u->servername + " " + u->nickname + " " + "H";
		if (this->m_ops.find(u->id) != this->m_ops.end())
			msg_end += "*";
		else
			msg_end += " ";
		if (chan_ops.find(u->id) != chan_ops.end())
			msg_end += "@ ";
		msg_end += ":0 " + u->realname;
		user.send(msg + msg_end + "\n\r");
	}
	this->rplEndOfWho(user, mask);
}
