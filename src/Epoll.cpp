#include "Epoll.hpp"
#include <stdexcept>

Epoll::Epoll(uint timeout) : m_timeout(timeout)
{
	m_epollFd = epoll_create(DEFAULT_SIZE);
	if (m_epollFd == -1) {
		throw std::runtime_error("Failed to create epoll file descriptor");
	}
}

Epoll::~Epoll() throw()
{
	close(m_epollFd);
}

bool Epoll::ctl(int op, int fd, EpollEvent &event) throw()
{
	return ::epoll_ctl(m_epollFd, op, fd, &event) != -1;
}

bool Epoll::ctlAdd(int fd, uint32_t events) throw()
{
	EpollEvent event;
	event.events = events;
	event.data.fd = fd;
	return this->ctl(EPOLL_CTL_ADD, fd, event);
}

bool Epoll::ctlMod(int fd, uint32_t events) throw()
{
	struct epoll_event event;
	event.events = events;
	event.data.fd = fd;
	return this->ctl(EPOLL_CTL_MOD, fd, event);
}

bool Epoll::ctlDel(int fd) throw()
{
	struct epoll_event event;
	event.events = 0;
	event.data.fd = 0;
	return this->ctl(EPOLL_CTL_DEL, fd, event);
}

ssize_t Epoll::wait(std::vector<EpollEvent> &buf) throw()
{
	return ::epoll_wait(m_epollFd, buf.data(), buf.size(), m_timeout);
}

std::ostream &operator<<(std::ostream &os, const struct epoll_event &event)
{
	os << "fd: " << event.data.fd << ", event: [" << ((event.events & EPOLLIN) ? "EPOLLIN, " : "")
	   << ((event.events & EPOLLOUT) ? "EPOLLOUT, " : "")
	   << ((event.events & EPOLLHUP) ? "EPOLLHUP, " : "")
	   << ((event.events & EPOLLET) ? "EPOLLET, " : "")
	   << ((event.events & EPOLLERR) ? "EPOLLERR, " : "") << "]";
	return os;
}
