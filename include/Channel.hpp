#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <set>
#include <string>

class Server;

struct Channel
{
	const size_t id;
	std::string topic;
	std::set<size_t> ops;
	std::set<size_t> users;

	explicit Channel(const std::string &name, Server &server);
	~Channel();
	void broadcast(const std::string &msg) const;
	void privmsg(const size_t &user_id, const std::string &msg) const;
	const std::string &name() const throw();

private:
	Server &m_server;
	std::string m_name;

	static size_t createId() throw();
};

#endif /* CHANNEL_HPP */
