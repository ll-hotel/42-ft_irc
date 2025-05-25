#pragma once

#include "Command.hpp"
#include "SocketAddr.hpp"
#include "TcpStream.hpp"
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

	User(const TcpStream &stream, const SocketAddr &addr, Server &server);
	~User();
	bool receive();
	void send(const std::string &message);
	bool flush();
	bool parseNextCommand();
	bool registered() const throw();

private:
	std::string m_streamBuffer;
	std::string m_sendBuffer;
	Server &m_server;

	static size_t createId() throw();
};
