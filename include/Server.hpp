#pragma once

#include "TcpSocket.hpp"
#include "User.hpp"
#include <string>
#include <vector>

class Server
{
private:
	TcpSocket m_socket;
	std::string m_password;
	bool m_running;
	std::vector<User *> m_users;

public:
	explicit Server(uint16_t port, const std::string &password);
	~Server();
	void run();
};
