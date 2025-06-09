#include "Server.hpp"
#include "utils.hpp"

static std::vector<User *> channelUsers(const Channel &channel, const Server &server,
										const bool op_only);
static std::vector<User *> matchingUsers(const std::string &pattern,
										 const std::vector<User *> &users,
										 const std::set<size_t> &ops, const bool op_only);

void Server::commandWho(const Command &command, User &client) const
{
	const bool op_only = (command.args.size() > 1 && command.args[1] == "o");
	std::string mask = (command.args.empty() ? "*" : command.args[0]);

	std::vector<User *> user_list;
	std::set<size_t> channel_ops;
	if (mask == "*") {
		user_list = m_users;
	}
	else if (mask[0] == '#' || mask[0] == '&') {
		mask = strTolower(mask);
		std::vector<Channel *>::const_iterator chan_it = this->getChannelByName(mask);
		if (chan_it == m_channels.end()) {
			this->errNoSuchChannel(client, mask);
			return;
		}

		const Channel &channel = *(*chan_it);
		user_list = channelUsers(channel, *this, op_only);
		for (std::vector<User *>::const_iterator it = user_list.begin(); it != user_list.end();
			 it++) {
			const User &user = *(*it);
			if (channel.ops.find(user.id) != channel.ops.end()) {
				channel_ops.insert(user.id);
			}
		}
	}
	else {
		user_list = matchingUsers(mask, m_users, m_ops, op_only);
	}
	for (size_t i = 0; i < user_list.size(); i++) {
		if (user_list[i]->registered())
			this->rplWhoReply(client, *user_list[i], mask, channel_ops);
	}
	this->rplEndOfWho(client, mask);
}

static std::vector<User *> channelUsers(const Channel &channel, const Server &server,
										const bool op_only)
{
	std::vector<User *> users;
	for (std::set<size_t>::const_iterator user_id_it = channel.users.begin();
		 user_id_it != channel.users.end(); user_id_it++) {
		std::vector<User *>::const_iterator user_pos = server.getUserById(*user_id_it);
		if (user_pos != server.getUsers().end()) {
			User &channel_user = *(*user_pos);
			if (not op_only or server.isUserOp(channel_user, channel)) {
				users.push_back(&channel_user);
			}
		}
	}
	return users;
}

static std::vector<User *> matchingUsers(const std::string &pattern,
										 const std::vector<User *> &users,
										 const std::set<size_t> &ops, const bool op_only)
{
	std::vector<User *> matching;
	for (std::vector<User *>::const_iterator user_it = users.begin(); user_it != users.end();
		 user_it++) {
		const User &user = *(*user_it);
		if (patternMatch(pattern, user.nickname) || patternMatch(pattern, user.hostname) ||
			patternMatch(pattern, user.username)) {
			if (!op_only || ops.find(user.id) != ops.end()) {
				matching.push_back(*user_it);
			}
		}
	}
	return matching;
}
