#pragma once

#include "Epoll.hpp"
#include "TcpSocket.hpp"
#include "User.hpp"
#include <string>
#include <vector>

enum NumericReplyCode { ERR_NEEDMOREPARAMS = 461, ERR_ALREADYREGISTERED = 462 };

class Server
{
private:
	TcpSocket m_socket;
	std::string m_password;
	bool m_running;
	std::vector<User *> m_users;
	Epoll m_epoll;

public:
	explicit Server(uint16_t port, const std::string &password);
	~Server();
	void run();
	std::vector<User *>::iterator getUserByFd(int fd) throw();
	void routine();
	void processCommand(const Command &command, User &user);
	void reply(const NumericReplyCode code, const User &user) const;
	void commandPass(const Command &command, User &user) const;
};
