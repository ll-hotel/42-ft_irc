#pragma once

#include "TcpSocket.hpp"
#include <string>

class Server
{
private:
	TcpSocket m_socket;
	std::string m_password;
	bool m_running;

public:
	explicit Server(uint16_t port, const std::string &password);
	~Server();
	void run();
};
