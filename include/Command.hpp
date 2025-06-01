#pragma once

#include <ostream>
#include <string>
#include <vector>

struct Command
{
	std::string name;
	std::vector<std::string> args;
	enum Id { UNKNOWN, PASS, NICK, USER, JOIN, TOPIC, PART, PRIVMSG, NAMES, QUIT, KICK, INVITE } id;

	explicit Command();
	explicit Command(const std::string &input);
};

std::ostream &operator<<(std::ostream &os, const Command &command);
