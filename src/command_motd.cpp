#include "Server.hpp"

void Server::commandMOTD(const Command &cmd, User &user) const
{
	if (cmd.args.size() > 0) {
		if (cmd.args[0] != m_hostname) {
			this->errNoSuchServer(user, cmd.args[0]);
			return;
		}
	}
	if (m_motd.empty()) {
		this->errNoMOTD(user);
		return;
	}
	this->rplMOTDStart(user);

	size_t line_pos = 0;
	std::string line;
	do {
		size_t newline = m_motd.find("\n", line_pos);
		line = m_motd.substr(line_pos, newline - line_pos);
		if (newline != std::string::npos) {
			line_pos += line.size() + 1;
		}
		else {
			line_pos = newline;
		}
		this->rplMOTD(user, line);
	} while (line_pos < m_motd.size());

	this->rplEndOfMOTD(user);
}
