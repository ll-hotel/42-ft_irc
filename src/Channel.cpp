#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(const std::string &name) : id(createId()), m_name(name)
{
}

Channel::~Channel()
{
}

bool Channel::addUser(User &user)
{
	m_users.push_back(&user);
	return true;
}

bool Channel::removeUser(User &user)
{
	for (size_t i = 0; i < m_users.size(); i += 1) {
		if (m_users[i]->username == user.username) {
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

const std::string &Channel::name() const throw()
{
	return m_name;
}

const std::vector<User *> &Channel::users() const throw()
{
	return m_users;
}

size_t Channel::createId() throw()
{
	static size_t current_id = 0;

	const size_t id = current_id;
	current_id += 1;
	return id;
}
