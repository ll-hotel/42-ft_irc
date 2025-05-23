#ifndef WEBSERV_TCP_SOCKET_HPP
#define WEBSERV_TCP_SOCKET_HPP

#include "SocketAddr.hpp"
#include "TcpStream.hpp"
#include <stdint.h>
#include <unistd.h>
#include <utility>

struct TcpSocket {
	explicit TcpSocket(uint16_t port);
	~TcpSocket() throw();

	int32_t rawFd() const throw();
	std::pair<TcpStream, SocketAddr> accept() const;

private:
	int32_t m_fd;

	explicit TcpSocket(int32_t rawFd) throw();
	TcpSocket();
	TcpSocket(TcpSocket &);
	TcpSocket &operator=(TcpSocket &);
};

#endif /* WEBSERV_TCP_SOCKET_HPP */
