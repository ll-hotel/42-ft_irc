#include "SocketAddr.hpp"
#include <cstring>
#include <sys/socket.h>

SocketAddr::SocketAddr() throw() : m_kind(V6)
{
	std::memset(&m_v6, 0, sizeof(m_v6));
}

SocketAddr::SocketAddr(const struct sockaddr &addr) throw()
{
	*this = SocketAddr();
	if (addr.sa_family == AF_INET) {
		m_kind = V4;
		m_v4 = (const struct sockaddr_in &)addr.sa_data;
	}
	else {
		m_kind = V6;
		m_v6 = (const struct sockaddr_in6 &)addr.sa_data;
	}
}

const uint8_t (&SocketAddr::ipv4() const)[4]
{
	union CastUnion {
		uint32_t i;
		uint8_t bytes[4];
	};
	return ((const CastUnion &)m_v4.sin_addr.s_addr).bytes;
}

const uint8_t (&SocketAddr::ipv6() const)[16]
{
	return m_v6.sin6_addr.s6_addr;
}
