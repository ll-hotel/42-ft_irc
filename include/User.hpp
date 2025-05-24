#pragma once

#include "Command.hpp"
#include "SocketAddr.hpp"
#include "TcpStream.hpp"
#include <string>

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

	User(const TcpStream &stream, const SocketAddr &addr);
	~User();
	bool receive();
	void send(const std::string &message);
	bool flush();
	bool parseNextCommand();
	bool registered() const throw();

private:
	std::string m_streamBuffer;
	std::string m_sendBuffer;
};
