#pragma once

#include "Channel.hpp"
#include "Command.hpp"
#include "Epoll.hpp"
#include "TcpSocket.hpp"
#include "User.hpp"
#include "reply_info.hpp"
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
	std::set<size_t> m_ops;

public:
	explicit Server(uint16_t port, const std::string &password);
	~Server();
	void run();
	std::vector<User *>::iterator getUserByFd(int fd) throw();
	std::vector<User *>::iterator getUserById(size_t id) throw();
	std::vector<Channel *>::iterator getChannelByName(const std::string &name) throw();
	std::vector<Channel *>::iterator getChannelById(size_t id) throw();
	std::vector<Channel *>::const_iterator getChannelByName(const std::string &name) const throw();
	std::vector<User *>::const_iterator getUserbyNickname(const std::string &name) const throw();
	void routine();
	void delUser(size_t id);
	void delChannel(size_t id);
	void removeChannelUser(Channel &chan, User &user) const;
	void connectUserToChannel(User &user, Channel &chan) const;
	void processCommand(const Command &command, User &user);
	std::string getReplyBase(const NumericReplyCode code, const User &user) const;
	void reply(const NumericReplyCode code, User &user) const;
	void commandPass(const Command &command, User &user) const;
	void commandNick(const Command &command, User &user) const;
	void commandUser(const Command &command, User &user) const;
	void commandJoin(const Command &command, User &user);
	void commandQuit(const Command &command, User &user);
	void commandMode(const Command &command, User &user);
	void commandNames(const Command &command, User &user) const;
	// implement me is_user_op
	void commandTopic(const Command &command, User &user);
	void commandPart(const Command &command, User &user);
	void commandPrivMsg(const Command &command, User &user) const;
	void commandKick(const Command &command, User &user) const;
	void errNoSuchChannel(User &user, const std::string &channel_name) const;
	void errNotOnChannel(User &user, const std::string &channel_name) const;
	void errNoRecipient(User &user, const std::string &cmd) const;
	void errNoTextToSend(User &user) const;
	void errNoSuchNick(User &user, const std::string &nick) const;
	void errNeedMoreParams(User &user, const std::string &cmd) const;
	void errChanOPrivsNeeded(User &user, const std::string &channel_name) const;
	void errUnknownMode(User &user, const std::string &channel_name, std::string c) const;
	void errKeySet(User &user, const std::string &channel_name) const;
	void errUserNotInChannel(User &user, const std::string &nick,
							 const std::string &channel_name) const;
	void rplEndOfNames(User &user, const std::string &chan) const;
	void rplNamReply(User &user, const std::string &chan) const;

	const std::vector<User *> &getUsers() const;
	const std::vector<Channel *> &getChannels() const;
	const std::set<size_t> &getOps() const;
	bool isUserOp(const User &user, const Channel &channel);
	void cleanChannel();
};
