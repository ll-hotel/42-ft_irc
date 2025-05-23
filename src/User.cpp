#include "User.hpp"
#include <iostream>
#include <ostream>

User::User(const TcpStream &stream, const SocketAddr &addr)
	: stream(stream), addr(addr), name(), nick(), nextCommand(), authenticated(false),
	  m_streamBuffer()
{
	std::cerr << "user " << this->stream.rawFd() << " created" << std::endl;
}

User::~User()
{
	std::cerr << "user " << stream.rawFd() << " deleted" << std::endl;
}

bool User::receive()
{
	static char recv_buffer[1024] = {0};
	const ssize_t recv_size = this->stream.recv(recv_buffer, sizeof(recv_buffer) - 1);
	if (recv_size <= 0)
		return false;
	recv_buffer[recv_size] = '\0';
	this->m_streamBuffer.append(recv_buffer);
	return true;
}

bool User::parseNextCommand()
{
	const size_t crlf = this->m_streamBuffer.find("\r\n");
	if (crlf == std::string::npos)
		return false;
	std::cout << this->m_streamBuffer.substr(0, crlf) << std::endl;
	this->nextCommand = Command(this->m_streamBuffer.substr(0, crlf));
	this->m_streamBuffer.erase(0, crlf + 2);
	return true;
}
