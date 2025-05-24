#pragma once

#include "Epoll.hpp"
#include "TcpSocket.hpp"
#include "User.hpp"
#include <string>
#include <vector>

enum NumericReplyCode {
	// PASS
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	// NICK
	ERR_NONICKNAMEGIVEN = 431,
	// ERR_ERRONEUSNICKNAME = 432, /* Not used, because we do not have a nickname 'blacklist' */
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436
	// USER
	// ERR_NEEDMOREPARAMS
	// ERR_ALREADYREGISTERED
};

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
	void reply(const NumericReplyCode code, User &user) const;
	void commandPass(const Command &command, User &user) const;
	void commandNick(const Command &command, User &user) const;
	void commandUser(const Command &command, User &user) const;
};
