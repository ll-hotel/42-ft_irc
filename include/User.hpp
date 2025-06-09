#pragma once

#include "Command.hpp"
#include "Epoll.hpp"
#include "SocketAddr.hpp"
#include "TcpStream.hpp"
#include <set>
#include <string>

class Server;

struct User
{
	TcpStream stream;
	SocketAddr addr;
	Command nextCommand;
	bool didPass;
	bool didNick;
	bool didUser;
	std::string nickname;
	std::string username;
	std::string hostname;
	std::string servername;
	std::string realname;
	const size_t id;
	std::set<size_t> channels;
	bool quit;
	std::string quitMessage;
	std::set<size_t> inviteList;

	User(const TcpStream &stream, const SocketAddr &addr, Epoll &server);
	~User();
	bool receive();
	void send(const std::string &message);
	bool flush();
	bool parseNextCommand();
	bool registered() const throw();
	std::string clientName(const std::string &host) const;
	bool isInvalidCommand() const;

private:
	bool m_invalidCommand;
	std::string m_readBuffer;
	std::string m_sendBuffer;
	Epoll &m_epoll;

	static size_t createId() throw();
};
