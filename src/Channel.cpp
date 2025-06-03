#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"

Channel::Channel(const std::string &name, Server &server)
	: id(createId()), inviteOnly(false), m_server(server), m_name(name)
{
	topic_op_only = true;
	password_set = false;
	invite_only = false;
	limit_user = -1;
}

Channel::~Channel()
{
}

void Channel::broadcast(const std::string &msg) const
{
	std::set<size_t>::iterator it = users.begin();
	std::set<size_t>::iterator it_end = users.end();
	while (it != it_end) {
		std::vector<User *>::iterator user_it = m_server.getUserById(*it);
		if (user_it != m_server.getUsers().end())
			(*user_it)->send(msg);
		it++;
	}
}

void Channel::privmsg(const size_t &sender_id, const std::string &msg) const
{
	std::set<size_t>::iterator it = users.begin();
	std::set<size_t>::iterator it_end = users.end();
	while (it != it_end) {
		std::vector<User *>::iterator user_it = m_server.getUserById(*it);
		if (user_it != m_server.getUsers().end()) {
			User &user = *(*user_it);
			if (user.id != sender_id) {
				user.send(msg);
			}
		}
		it++;
	}
}

const std::string &Channel::name() const throw()
{
	return m_name;
}

size_t Channel::createId() throw()
{
	static size_t current_id = 0;

	const size_t id = current_id;
	current_id += 1;
	return id;
}
