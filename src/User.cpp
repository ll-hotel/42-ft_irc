#include "User.hpp"
#include <iostream>

User::User(const TcpStream &stream, const SocketAddr &addr)
	: stream(stream), addr(addr), didPass(false), didNick(false), didUser(false)
{
	std::cerr << "User " << this->stream.rawFd() << " created" << std::endl;
}

User::~User()
{
	std::cerr << "User " << stream.rawFd() << " deleted" << std::endl;
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

void User::send(const std::string &message)
{
	m_sendBuffer += message;
}

bool User::flush()
{
	ssize_t len = stream.send(m_sendBuffer.c_str(), m_sendBuffer.size());
	if (len == -1)
		return (false);
	m_sendBuffer.erase(0, len);
	return (true);
}

bool User::parseNextCommand()
{
	const size_t crlf = this->m_streamBuffer.find("\r\n");
	if (crlf == std::string::npos)
		return false;
	this->nextCommand = Command(this->m_streamBuffer.substr(0, crlf));
	this->m_streamBuffer.erase(0, crlf + 2);
	return true;
}
