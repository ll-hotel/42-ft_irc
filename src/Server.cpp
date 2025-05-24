#include "Server.hpp"
#include "Command.hpp"
#include "Epoll.hpp"
#include <iostream>
#include <stdexcept>
#include <stdint.h>

Server::Server(uint16_t port, const std::string &password)
	: m_socket(port), m_epoll(1000), m_password(password), m_running(true), m_hostname("localhost")
{
	m_epoll.ctlAdd(m_socket.rawFd(), EPOLLIN);
}

Server::~Server()
{
	for (size_t i = 0; i < m_users.size(); i += 1) {
		m_epoll.ctlDel(m_users[i]->stream.rawFd());
		delete m_users[i];
	}
	m_users.clear();
	for (size_t i = 0; i < m_channels.size(); i += 1) {
		delete m_channels[i];
	}
	m_channels.clear();
}

extern bool sigint_received;

void Server::run()
try {
	while (m_running and not sigint_received)
		routine();
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
			User *user = new User(tmp.first, tmp.second);
			m_users.push_back(user);
			m_epoll.ctlAdd(user->stream.rawFd(), EPOLLIN);
		}
		else {
			std::vector<User *>::iterator user_pos = getUserByFd(event.data.fd);
			if (user_pos == m_users.end()) {
				std::cerr << "No such client" << std::endl;
				m_epoll.ctlDel(event.data.fd);
				delete *user_pos;
				m_users.erase(user_pos);
				continue;
			}
			if (event.events & (EPOLLERR | EPOLLHUP)) {
				m_epoll.ctlDel(event.data.fd);
				delete *user_pos;
				m_users.erase(user_pos);
				continue;
			}
			User &user = **user_pos;
			if (event.events & EPOLLIN) {
				if (!user.receive()) {
					m_epoll.ctlDel(event.data.fd);
					delete *user_pos;
					m_users.erase(user_pos);
					continue;
				}
				while (user.parseNextCommand()) {
					std::cerr << user.nextCommand << std::endl;
					this->processCommand(user.nextCommand, user);
				}
				m_epoll.ctlMod(event.data.fd, EPOLLIN | EPOLLOUT);
			}
			if (event.events & EPOLLOUT) {
				try {
					if (user.flush())
						m_epoll.ctlMod(event.data.fd, EPOLLIN);
				} catch (const std::runtime_error &e) {
					std::cerr << e.what() << std::endl;
					m_epoll.ctlDel(event.data.fd);
					delete *user_pos;
					m_users.erase(user_pos);
					continue;
				}
			}
		}
	}
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
	case Command::UNKNOWN:
	default:
		break;
	}
}

void Server::reply(const NumericReplyCode code, User &user) const
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

void Server::commandPass(const Command &command, User &user) const
{
	if (command.args.size() < 1) {
		this->reply(ERR_NEEDMOREPARAMS, user);
		return;
	}
	if (command.args[0] != m_password)
		return;
	if (user.didPass) {
		this->reply(ERR_ALREADYREGISTERED, user);
		return;
	}
	user.didPass = true;
	std::cerr << "User " << user.stream.rawFd() << " authenticated" << std::endl;
}

void Server::commandNick(const Command &command, User &user) const
{
	if (command.args.size() < 1) {
		this->reply(ERR_NONICKNAMEGIVEN, user);
		return;
	}
	const std::string &new_nick = command.args[0];
	if (user.nickname == new_nick) {
		this->reply(ERR_NICKNAMEINUSE, user);
		return;
	}
	for (size_t i = 0; i < m_users.size(); i += 1) {
		if (m_users[i]->nickname == new_nick) {
			this->reply(ERR_NICKCOLLISION, user);
			return;
		}
	}
	user.nickname = new_nick;
	user.didNick = true;
	if (user.registered())
		this->reply(RPL_WELCOME, user);
}

void Server::commandUser(const Command &command, User &user) const
{
	if (user.didUser) {
		this->reply(ERR_ALREADYREGISTERED, user);
		return;
	}
	if (command.args.size() != 4) {
		this->reply(ERR_NEEDMOREPARAMS, user);
		return;
	}
	user.username = command.args[0];
	user.hostname = command.args[1];
	user.servername = command.args[2];
	user.realname = command.args[3];
	user.didUser = true;
	if (user.registered())
		this->reply(RPL_WELCOME, user);
}

std::vector<Channel *>::iterator Server::getChannelByName(const std::string &name) throw()
{
	for (std::vector<Channel *>::iterator it = m_channels.begin(); it != m_channels.end(); it++) {
		if ((*it)->name() == name)
			return it;
	}
	return m_channels.end();
}
