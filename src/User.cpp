#include "User.hpp"

User::User(const TcpStream &stream, const SocketAddr &addr) : stream(stream), addr(addr)
{
}

User::~User()
{
}
