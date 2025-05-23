#include "Server.hpp"
#include <stdint.h>

Server::Server(uint16_t port, const std::string &password)
	: m_socket(port), m_password(password), m_running(true)
{
}

Server::~Server()
{
}

void Server::run()
{
	while (m_running) {
		std::pair<TcpStream, SocketAddr> client = m_socket.accept();

		char buf[4096];
		while (1) {
			ssize_t len = client.first.recv(buf, 4096);
			write(1, buf, len);
		}
	}
}
