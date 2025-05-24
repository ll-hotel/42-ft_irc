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

	bool addUser(User &user);
	bool removeUser(User &user);
	void sendMsg(const std::string &msg);
	const std::string &name() const throw();
	const std::vector<User *> &users() const throw();

private:
	std::vector<User *> m_users;
	std::string m_name;

	static size_t createId() throw();
};

#endif /* CHANNEL_HPP */
