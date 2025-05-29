#include "Server.hpp"
#include "Command.hpp"
#include "Epoll.hpp"
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <unistd.h>

Server::Server(uint16_t port, const std::string &password)
	: m_socket(port), m_epoll(1000), m_password(password), m_running(true), m_hostname("localhost")
{
	m_epoll.ctlAdd(m_socket.rawFd(), EPOLLIN);
}

Server::~Server()
{
	m_epoll.ctlDel(m_socket.rawFd());
	while (not m_users.empty()) {
		delete m_users.front();
		m_users.erase(m_users.begin());
	}
	while (not m_channels.empty()) {
		delete m_channels.front();
		m_channels.erase(m_channels.begin());
	}
}

extern bool sigint_received;

void Server::run()
try {
	while (m_running and not sigint_received) {
		this->routine();
		this->cleanChannel();
	}
} catch (const std::exception &e) {
	std::cerr << e.what() << std::endl;
}

std::vector<User *>::iterator Server::getUserByFd(int fd) throw()
{
	for (size_t i = 0; i < m_users.size(); i++)
		if (m_users[i]->stream.rawFd() == fd)
			return m_users.begin() + i;
	return m_users.end();
}

std::vector<User *>::iterator Server::getUserById(size_t id) throw()
{
	for (size_t i = 0; i < m_users.size(); i++)
		if (m_users[i]->id == id)
			return m_users.begin() + i;
	return m_users.end();
}

void Server::routine()
{
	std::vector<EpollEvent> events(20);
	ssize_t action_count = m_epoll.wait(events);
	if (action_count == -1) {
		m_running = false;
		throw std::runtime_error("epoll: wait");
	}
	for (ssize_t cur_act = 0; cur_act < action_count; cur_act++) {
		const EpollEvent &event = events[cur_act];
		std::cerr << event << std::endl;
		if (event.data.fd == m_socket.rawFd()) {
			std::pair<TcpStream, SocketAddr> tmp = m_socket.accept();
			User *user = new User(tmp.first, tmp.second, m_epoll);
			m_users.push_back(user);
			m_epoll.ctlAdd(user->stream.rawFd(), EPOLLIN);
			continue;
		}
		std::vector<User *>::iterator user_pos = getUserByFd(event.data.fd);
		if (user_pos == m_users.end()) {
			std::cerr << "No such client" << std::endl;
			::close(event.data.fd);
			m_epoll.ctlDel(event.data.fd);
			continue;
		}
		User &user = *(*user_pos);
		if (event.events & (EPOLLERR | EPOLLHUP)) {
			this->delUser(user.id);
			continue;
		}
		if (event.events & EPOLLIN) {
			if (!user.receive()) {
				this->delUser(user.id);
				continue;
			}
			while (user.parseNextCommand()) {
				std::cerr << user.nickname << ": " << user.nextCommand << std::endl;
				this->processCommand(user.nextCommand, user);
			}
			if (user.quit)
				this->delUser(user.id);
		}
		if (event.events & EPOLLOUT) {
			try {
				user.flush();
			} catch (const std::runtime_error &e) {
				std::cerr << e.what() << std::endl;
				continue;
			}
		}
	}
}

void Server::delUser(const size_t id)
{
	std::vector<User *>::iterator user_pos = this->getUserById(id);
	if (user_pos == m_users.end()) {
		std::cerr << "WARN: try to delete invalid User" << std::endl;
		return;
	}
	User *const user_ptr = (*user_pos);
	if (user_ptr == NULL) {
		std::cerr << "WARN: try to delete invalid User 'NULL'" << std::endl;
	}
	else {
		User &user = (*user_ptr);
		m_epoll.ctlDel(user.stream.rawFd());
		const std::string msg = ':' + user.nickname + '!' + user.username + '@' + m_hostname + ' ' +
								(user.quit ? "QUIT" : "PART") + ' ';

		while (not user.channels.empty()) {
			const size_t chan_id = (*user.channels.begin());
			std::vector<Channel *>::const_iterator const chan_pos = this->getChannelById(chan_id);
			if (chan_pos == m_channels.end()) {
				continue;
			}
			Channel &chan = *(*chan_pos);
			this->removeChannelUser(chan, user);
			std::string sendmsg;
			if (user.quit)
				sendmsg = msg + user.quitMessage;
			else
				sendmsg = msg + chan.name() + " :Disconnected";
			chan.broadcast(sendmsg + "\r\n");
		}
		delete user_ptr;
		m_users.erase(user_pos);
	}
}

void Server::delChannel(const size_t id)
{
	std::vector<Channel *>::iterator chan_pos = this->getChannelById(id);
	if (chan_pos == m_channels.end()) {
		std::cerr << "WARN: try to delete invalid chan" << std::endl;
		return;
	}
	Channel *const chan_ptr = (*chan_pos);
	if (chan_ptr == NULL) {
		std::cerr << "WARN: try to delete invalid chan 'NULL'" << std::endl;
	}
	else {
		Channel &chan = (*chan_ptr);
		std::cerr << "Removing channel " << chan.name() << std::endl;
		delete chan_ptr;
	}
	m_channels.erase(chan_pos);
}

void Server::removeChannelUser(Channel &chan, User &user) const
{
	chan.users.erase(user.id);
	user.channels.erase(chan.id);
}

std::vector<Channel *>::iterator Server::getChannelById(const size_t id) throw()
{
	for (std::vector<Channel *>::iterator chan_it = m_channels.begin(); chan_it != m_channels.end();
		 chan_it++) {
		Channel &chan = *(*chan_it);
		if (chan.id == id) {
			return chan_it;
		}
	}
	return m_channels.end();
}

void Server::processCommand(const Command &command, User &user)
{
	if (command.id == Command::PASS) {
		this->commandPass(command, user);
		return;
	}
	else if (not user.didPass) {
		std::cerr << "User " << user.stream.rawFd() << " tried to " << command.name
				  << " while not authenticated" << std::endl;
		return;
	}
	if (command.id == Command::NICK) {
		this->commandNick(command, user);
		return;
	}
	else if (command.id == Command::USER) {
		this->commandUser(command, user);
		return;
	}
	else if (not user.registered()) {
		std::cerr << "User " << user.stream.rawFd() << " tried to " << command.name
				  << " while not registered" << std::endl;
		return;
	}
	switch (command.id) {
	case Command::JOIN:
		this->commandJoin(command, user);
		break;
	case Command::TOPIC:
		this->commandTopic(command, user);
		break;
	case Command::PART:
		this->commandPart(command, user);
		break;
	case Command::PRIVMSG:
		this->commandPrivMsg(command, user);
		break;
	case Command::NAMES:
		this->commandNames(command, user);
		break;
	case Command::QUIT:
		this->commandQuit(command, user);
		break;
	case Command::KICK:
		this->commandKick(command, user);
		break;
	case Command::UNKNOWN:
	default:
		break;
	}
}

std::string Server::getReplyBase(const NumericReplyCode code, const User &user) const
{
	std::string reply_str;
	reply_str.push_back(':');
	reply_str.append(m_hostname);
	reply_str.push_back(' ');
	reply_str.append(ft_ltoa(code));
	reply_str.push_back(' ');
	if (user.nickname.empty())
		reply_str.push_back('*');
	else
		reply_str.append(user.nickname);
	return (reply_str);
}

void Server::reply(const NumericReplyCode code, User &user) const
{
	std::string reply_str = getReplyBase(code, user);

	reply_str.push_back(' ');

	if (code == ERR_NEEDMOREPARAMS) {
		reply_str.append(":Need more params");
	}
	else if (code == ERR_ALREADYREGISTERED) {
		reply_str.append(":Already registered");
	}
	else if (code == ERR_NONICKNAMEGIVEN) {
		reply_str.append(":No nickname given");
	}
	else if (code == ERR_NICKNAMEINUSE) {
		reply_str.append(":Nickname already in use");
	}
	else if (code == ERR_NICKCOLLISION) {
		reply_str.append(":Nickname collision");
	}
	else if (code == RPL_WELCOME) {
		reply_str.append(":Welcome to our IRC server!");
	}
	reply_str.append("\r\n");
	user.send(reply_str);
}

std::vector<Channel *>::iterator Server::getChannelByName(const std::string &name) throw()
{
	for (std::vector<Channel *>::iterator it = m_channels.begin(); it != m_channels.end(); it++) {
		if ((*it)->name() == name)
			return it;
	}
	return m_channels.end();
}

const std::vector<User *> &Server::getUsers() const
{
	return (m_users);
}

const std::vector<Channel *> &Server::getChannels() const
{
	return (m_channels);
}

void Server::cleanChannel()
{
	bool foundEmptyChannel = true;
	while (foundEmptyChannel) {
		foundEmptyChannel = false;
		for (std::vector<Channel *>::iterator chan_it = m_channels.begin();
			 not foundEmptyChannel and chan_it != m_channels.end(); chan_it++) {
			Channel &chan = *(*chan_it);
			if (chan.users.empty()) {
				this->delChannel(chan.id);
				foundEmptyChannel = true;
			}
		}
	}
}

void Server::connectUserToChannel(User &user, Channel &chan) const
{
	if (chan.users.empty() and chan.ops.empty()) {
		chan.ops.insert(user.id);
	}
	chan.users.insert(user.id);
	user.channels.insert(chan.id);
}

std::vector<Channel *>::const_iterator Server::getChannelByName(const std::string &name) const
	throw()
{
	for (std::vector<Channel *>::const_iterator it = m_channels.begin(); it != m_channels.end();
		 it++) {
		if ((*it)->name() == name)
			return it;
	}
	return m_channels.end();
}

std::vector<User *>::const_iterator Server::getUserbyNickname(const std::string &nick) const throw()
{
	for (std::vector<User *>::const_iterator it = m_users.begin(); it != m_users.end(); it++) {
		if ((*it)->nickname == nick)
			return it;
	}
	return m_users.end();
}
