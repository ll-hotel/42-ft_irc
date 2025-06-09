#include "User.hpp"
#include "TcpSocket.hpp"
#include <stdexcept>

#define USER_BUFFER_SIZE_LIMIT 4096
#define USER_REQUEST_SIZE_LIMIT 512

User::User(const TcpStream &stream, const SocketAddr &addr, Epoll &epoll)
	: stream(stream), addr(addr), didPass(false), didNick(false), didUser(false), id(createId()),
	  quit(false), m_invalidCommand(false), m_epoll(epoll)
{
}

User::~User()
{
}

bool User::receive()
{
	if (m_readBuffer.size() >= USER_BUFFER_SIZE_LIMIT)
		return true;
	static char recv_buffer[1024] = {0};
	const ssize_t recv_size = this->stream.recv(recv_buffer, sizeof(recv_buffer) - 1);
	if (recv_size <= 0)
		return false;
	recv_buffer[recv_size] = '\0';
	m_readBuffer.append(recv_buffer);
	return true;
}

void User::send(const std::string &message)
{
	if (m_sendBuffer.empty())
		m_epoll.ctlMod(stream.rawFd(), EPOLLIN | EPOLLOUT);
	m_sendBuffer += message;
}

bool User::flush()
{
	ssize_t len = stream.send(m_sendBuffer.c_str(), m_sendBuffer.size());
	if (len == -1)
		throw std::runtime_error("Could not send to " + ft_ltoa(this->stream.rawFd()));
	const bool fully_flushed = (size_t)len == m_sendBuffer.size();
	m_sendBuffer.erase(0, len);
	if (fully_flushed)
		m_epoll.ctlMod(this->stream.rawFd(), EPOLLIN);
	return (fully_flushed);
}

bool User::parseNextCommand()
{
	const size_t crlf = m_readBuffer.find("\r\n");
	if (m_invalidCommand or crlf == std::string::npos or crlf > USER_REQUEST_SIZE_LIMIT) {
		m_readBuffer.erase(0, crlf);
		if (crlf == std::string::npos)
			m_invalidCommand = true;
		else
			m_invalidCommand = false;
		return false;
	}
	this->nextCommand = Command(m_readBuffer.substr(0, crlf));
	m_readBuffer.erase(0, crlf + 2);
	return true;
}

bool User::isInvalidCommand() const
{
	return m_invalidCommand;
}

bool User::registered() const throw()
{
	return didUser and didNick and didPass;
}

size_t User::createId() throw()
{
	static size_t current_id = 0;

	const size_t id = current_id;
	current_id += 1;
	return id;
}

std::string User::clientName(const std::string &host) const
{
	return (this->nickname.empty() ? "*" : this->nickname) + '!' + this->username + '@' + host;
}
