#ifndef SOCKET_ADDR_HPP
#define SOCKET_ADDR_HPP

#include <netinet/in.h>
#include <stdint.h>

struct SocketAddr {
	explicit SocketAddr() throw();
	explicit SocketAddr(struct sockaddr const &) throw();

	enum IpVersion { V4, V6 };
	const IpVersion &kind() throw();
	const uint8_t (&ipv4() const)[4];
	const uint8_t (&ipv6() const)[16];

private:
	IpVersion m_kind;
	union {
		struct sockaddr_in m_v4;
		struct sockaddr_in6 m_v6;
	};
};

#endif /* SOCKET_ADDR_HPP */
