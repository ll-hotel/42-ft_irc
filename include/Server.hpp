#pragma once

#include "Channel.hpp"
#include "Epoll.hpp"
#include "TcpSocket.hpp"
#include "User.hpp"
#include "reply_info.hpp"
#include <iostream>
#include <string>
#include <vector>

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
	std::vector<User *>::iterator getUserById(size_t id) throw();
	void routine();
	void processCommand(const Command &command, User &user);
	std::string getReplyBase(const NumericReplyCode code, const User &user) const;
	void reply(const NumericReplyCode code, User &user) const;
	void commandPass(const Command &command, User &user) const;
	void commandNick(const Command &command, User &user) const;
	void commandUser(const Command &command, User &user) const;
	std::vector<Channel *>::iterator getChannelByName(const std::string &name) throw();
	void commandJoin(const Command &command, User &user);
	void commandNames(const Channel &chan, User &user) const;
	// implement me is_user_op
	void commandTopic(const Command &command, User &user);

	const std::vector<User *> &getUsers() const;
	const std::vector<Channel *> &getChannels() const;
	void cleanChannel();
};
