#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <string>
#include <vector>

struct Channel
{
	const size_t id;

	explicit Channel(const std::string &name);
	~Channel();

	bool addUser(const User &user);
	bool removeUser(const User &user);
	void sendMsg(const std::string &msg);
	const std::string &name() const throw();

private:
	std::string m_name;
	std::vector<const User *> m_users;

	static size_t createId() throw();
};

#endif /* CHANNEL_HPP */
