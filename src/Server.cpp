#include "Server.hpp"
#include "Epoll.hpp"
#include <iostream>
#include <stdexcept>
#include <stdint.h>

Server::Server(uint16_t port, const std::string &password)
	: m_socket(port), m_password(password), m_running(true), m_epoll(1000)
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
				do {
					const size_t crlf = user.streamBuffer.find("\r\n");
					if (crlf == std::string::npos)
						return;
					::write(1, user.streamBuffer.c_str(), crlf + 2);
					user.streamBuffer.erase(0, crlf + 2);
				} while (true);
			}
		}
	}
}
