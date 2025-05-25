#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"

Channel::Channel(const std::string &name, Server &server)
	: id(createId()), m_server(server), m_name(name)
{
}

Channel::~Channel()
{
}

bool Channel::addUser(User &user)
{
	m_users.insert(user.id);
	return true;
}

bool Channel::removeUser(User &user)
{
	removeOp(user.id);
	m_users.erase(user.id);
	return true;
}

void Channel::broadcast(const std::string &msg)
{
	std::set<size_t>::iterator it = m_users.begin();
	std::set<size_t>::iterator it_end = m_users.end();
	while (it != it_end) {
		std::vector<User *>::iterator user_it = m_server.getUserById(*it);
		if (user_it != m_server.getUsers().end())
			(*user_it)->send(msg);
		it++;
	}
}

const std::string &Channel::name() const throw()
{
	return m_name;
}

const std::set<size_t> &Channel::users() const throw()
{
	return m_users;
}

void Channel::addOp(size_t id)
{
	ops.insert(id);
}

void Channel::removeOp(size_t id)
{
	ops.erase(id);
}

size_t Channel::createId() throw()
{
	static size_t current_id = 0;

	const size_t id = current_id;
	current_id += 1;
	return id;
}
