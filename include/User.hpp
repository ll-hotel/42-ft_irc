#pragma once

#include "SocketAddr.hpp"
#include "TcpStream.hpp"
#include <string>

struct User
{
	TcpStream stream;
	SocketAddr addr;
	std::string username;
	std::string nickanme;

	explicit User(const TcpStream &stream, const SocketAddr &addr);
	~User();
};
