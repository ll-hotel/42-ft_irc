#include "User.hpp"

User::User(const TcpStream &stream, const SocketAddr &addr)
	: stream(stream), addr(addr), name(), nick(), streamBuffer()
{
}

User::~User()
{
}

bool User::receive()
{
	static char recv_buffer[1024] = {0};
	const ssize_t recv_size = this->stream.recv(recv_buffer, sizeof(recv_buffer) - 1);
	if (recv_size < 0)
		return false;
	recv_buffer[recv_size] = '\0';
	this->streamBuffer.append(recv_buffer);
	return true;
}
