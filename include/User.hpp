#pragma once

#include "SocketAddr.hpp"
#include "TcpStream.hpp"
#include <string>

struct User
{
	TcpStream stream;
	SocketAddr addr;
	std::string name;
	std::string nick;
	std::string streamBuffer;

	explicit User(const TcpStream &stream, const SocketAddr &addr);
	~User();
	bool receive();
};
