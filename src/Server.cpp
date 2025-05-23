#include "Server.hpp"
#include <iostream>
#include <stdint.h>

Server::Server(uint16_t port, const std::string &password)
	: m_socket(port), m_password(password), m_running(true), m_epoll(1000)
{
	m_epoll.ctlAdd(m_socket.rawFd(), EPOLLIN);
}

Server::~Server()
{
}

void Server::run()
{
	try {
		while (m_running) {
			routine();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

User *Server::getUserByFd(int fd) const throw()
{
	for (size_t i = 0; i < m_users.size(); i++)
		if (m_users[i]->stream.rawFd() == fd)
			return (m_users[i]);
	return (NULL);
}

void Server::routine()
{
	std::vector<EpollEvent> events(20);
	ssize_t action_count = m_epoll.wait(events);
	if (action_count == -1) {
		m_running = false;
		std::__throw_runtime_error("epoll: wait");
	}
	for (ssize_t cur_act = 0; cur_act < action_count; cur_act++) {
		if (events[cur_act].data.fd == m_socket.rawFd()) {

			std::pair<TcpStream, SocketAddr> tmp = m_socket.accept();
			User *user = new User(tmp.first, tmp.second);
			m_users.push_back(user);
			m_epoll.ctlAdd(user->stream.rawFd(), EPOLLOUT | EPOLLIN);
		}
		else {
			User *user = getUserByFd(events[cur_act].data.fd);
			if (user == NULL)
				std::cerr << "wtf" << std::endl;
			else {
				ssize_t len;
				char buf[4096];
				len = user->stream.recv(buf, 4096);
				write(1, buf, len);
			}
		}
	}
}
