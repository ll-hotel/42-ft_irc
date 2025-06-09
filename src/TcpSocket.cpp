#include "TcpSocket.hpp"
#include "TcpStream.hpp"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>

#define DEFAULT_PORT 80
#define DEFAULT_FAMILY AF_INET
#define DEFAULT_ADDR INADDR_ANY
#define DEFAULT_STREAM SOCK_STREAM
#define DEFAULT_LIMIT_QUEUE 256

TcpSocket::TcpSocket() : m_fd(-1)
{
}

TcpSocket::TcpSocket(TcpSocket &other) : m_fd(other.rawFd())
{
	other.m_fd = -1;
}

std::string ft_ltoa(int64_t n)
{
	std::stringstream stream;
	stream << n;
	return stream.str();
}

TcpSocket::TcpSocket(uint16_t port) : m_fd(-1)
{
	m_fd = ::socket(DEFAULT_FAMILY, DEFAULT_STREAM, 0);
	if (m_fd == -1) {
		throw std::runtime_error("Could not create TCP socket on port " + ft_ltoa(port));
	}
	const int optval = 1;
	if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) == -1) {
		throw std::runtime_error("Could not set socket option");
	}
	sockaddr_in address = {};
	address.sin_family = DEFAULT_FAMILY;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = DEFAULT_ADDR;
	if (::bind(m_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
		::close(m_fd);
		throw std::runtime_error("Could not bind TCP socket on port " + ft_ltoa(port));
	}
	if (::listen(m_fd, DEFAULT_LIMIT_QUEUE) == -1) {
		::close(m_fd);
		throw std::runtime_error("Could not listen on TCP socket");
	}
}

TcpSocket::~TcpSocket() throw()
{
	::shutdown(m_fd, SHUT_RDWR);
	::close(m_fd);
}

int32_t TcpSocket::rawFd() const throw()
{
	return m_fd;
}

std::pair<TcpStream, SocketAddr> TcpSocket::accept() const
{
	sockaddr addr = {};
	socklen_t t = 0;

	int32_t raw_fd = ::accept(m_fd, &addr, &t);
	if (raw_fd == -1) {
		throw std::runtime_error("Could not accept connection on TcpSocket");
	}
	const TcpStream connection(raw_fd);
	const SocketAddr remote_addr(addr);
	return std::pair<TcpStream, SocketAddr>(connection, remote_addr);
}

TcpSocket &TcpSocket::operator=(TcpSocket &other)
{
	this->~TcpSocket();
	m_fd = other.rawFd();
	other.m_fd = -1;
	return *this;
}
