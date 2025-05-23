#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(const std::string &name) : id(createId()), m_users()
{
}

Channel::~Channel()
{
}

bool Channel::addUser(const User &user)
{
	return false;
}

bool Channel::removeUser(const User &user)
{
	return false;
}

void Channel::sendMsg(const std::string &msg)
{
}

size_t Channel::createId() throw()
{
	static size_t current_id = 0;

	const size_t id = current_id;
	current_id += 1;
	return id;
}
