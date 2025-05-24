#pragma once

#include "Epoll.hpp"
#include "TcpSocket.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <string>
#include <vector>

enum NumericReplyCode {
	RPL_WELCOME = 1,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_NONICKNAMEGIVEN = 431,
	// ERR_ERRONEUSNICKNAME = 432, /* Not used, because we do not have a nickname 'blacklist' */
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	RPL_TOPIC = 332,
	RPL_NAMREPLY = 353
};

class Server
{
private:
	TcpSocket m_socket;
	Epoll m_epoll;
	std::vector<User *> m_users;
	std::vector<Channel *> m_channels;
	std::string m_password;
	bool m_running;
	std::string m_hostname;

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
	std::vector<Channel *>::iterator getChannelByName(const std::string &name) throw();
	void commandJoin(const Command &command, User &user);
	void commandNames(const Channel &chan, User &user) const;
};
