#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

typedef enum EPOLL_EVENTS EpollEvents;
typedef struct epoll_event EpollEvent;

struct Epoll
{
	static const int DEFAULT_MAX_EVENT = 2048;
	static const int DEFAULT_SIZE = 2048;

	explicit Epoll(uint timeout);
	~Epoll() throw();
	bool ctl(int op, int fd, EpollEvent &event) throw();
	bool ctlAdd(int fd, uint events) throw();
	bool ctlMod(int fd, uint events) throw();
	bool ctlDel(int fd) throw();
	ssize_t wait(std::vector<EpollEvent> &buf) throw();

private:
	int m_epollFd;
	uint m_timeout;

	Epoll(const Epoll &);
	Epoll &operator=(const Epoll &);
};

#endif /* EPOLL_HPP */
