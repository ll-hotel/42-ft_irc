#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(const std::string &name) : id(createId()), m_name(name), m_users()
{
}

Channel::~Channel()
{
}

bool Channel::addUser(const User &user)
{
	m_users.push_back(&user);
	return false;
}

bool Channel::removeUser(const User &user)
{
	for (size_t i = 0; i < m_users.size(); i += 1) {
		if (m_users[i]->name == user.name) {
			m_users.erase(m_users.begin() + i);
			return true;
		}
	}
	return false;
}

void Channel::sendMsg(const std::string &msg)
{
	for (size_t i = 0; i < m_users.size(); i += 1) {
		m_users[i]->stream.send(msg.data(), msg.size());
	}
}

size_t Channel::createId() throw()
{
	static size_t current_id = 0;

	const size_t id = current_id;
	current_id += 1;
	return id;
}
