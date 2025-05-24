#ifndef TCP_STREAM_HPP
#define TCP_STREAM_HPP

#include <cstdio>
#include <stdint.h>
#include <string>

struct TcpStream {
	explicit TcpStream() throw();
	explicit TcpStream(int32_t rawFd) throw();
	explicit TcpStream(const TcpStream &);
	~TcpStream() throw();
	TcpStream &operator=(const TcpStream &);

	const int32_t &rawFd() const throw();
	int32_t dupFd() const;
	ssize_t recv(char ptr[], size_t len) const throw();
	ssize_t send(const char ptr[], size_t len) const throw();
	ssize_t send(const std::string &) const throw();

private:
	int32_t m_fd;
};

#endif /* TCP_STREAM_HPP */
