#include "Server.hpp"
#include <stdint.h>

Server::Server(uint16_t port, const std::string &password) : m_socket(port), m_password(password)
{
}

Server::~Server()
{
}
