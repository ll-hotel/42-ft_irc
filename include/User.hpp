#pragma once

#include "Command.hpp"
#include "SocketAddr.hpp"
#include "TcpStream.hpp"
#include <string>

struct User
{
	TcpStream stream;
	SocketAddr addr;
	std::string name;
	std::string nick;
	Command nextCommand;
	bool authenticated;

	User(const TcpStream &stream, const SocketAddr &addr);
	~User();
	bool receive();
	bool parseNextCommand();

private:
	std::string m_streamBuffer;
};
