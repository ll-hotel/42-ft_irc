#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <set>
#include <string>
#include <vector>

class Server;

struct Channel
{
	const size_t id;
	std::string topic;
	std::set<size_t> ops;

	explicit Channel(const std::string &name, Server &server);
	~Channel();

	bool addUser(User &user);
	bool removeUser(User &user);
	void broadcast(const std::string &msg);
	const std::string &name() const throw();
	const std::set<size_t> &users() const throw();
	void addOp(size_t id);
	void removeOp(size_t id);

private:
	Server &m_server;
	std::set<size_t> m_users;
	std::string m_name;

	static size_t createId() throw();
};

#endif /* CHANNEL_HPP */
