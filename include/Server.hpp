#pragma once

#include "TcpSocket.hpp"
#include <string>

class Server
{
private:
	TcpSocket m_socket;
	std::string m_password;

public:
	Server(uint16_t port, const std::string &password);
	~Server();
};
