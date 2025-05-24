#include "TcpStream.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

TcpStream::TcpStream() throw() : m_fd(-1)
{
}

TcpStream::TcpStream(int32_t rawFd) throw() : m_fd(rawFd)
{
}

TcpStream::TcpStream(const TcpStream &other) : m_fd(other.dupFd())
{
}

TcpStream::~TcpStream() throw()
{
	::close(m_fd);
}

TcpStream &TcpStream::operator=(const TcpStream &other)
{
	this->~TcpStream();
	m_fd = other.dupFd();
	return *this;
}

const int32_t &TcpStream::rawFd() const throw()
{
	return m_fd;
}

int32_t TcpStream::dupFd() const
{
	const int32_t new_fd = ::dup(m_fd);
	if (new_fd == -1)
		throw std::runtime_error(std::string("dup: ") + strerror(errno));
	return new_fd;
}

ssize_t TcpStream::recv(char ptr[], size_t len) const throw()
{
	return ::recv(m_fd, ptr, len, 0);
}

ssize_t TcpStream::send(const char ptr[], size_t len) const throw()
{
	return ::send(m_fd, ptr, len, 0);
}

ssize_t TcpStream::send(const std::string &msg) const throw()
{
	return ::send(m_fd, msg.data(), msg.size(), 0);
}
