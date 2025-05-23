#pragma once

#include <string>
#include <vector>

struct Command
{
	std::string name;
	std::vector<std::string> args;
	enum Id { UNKNOWN, CAP, PASS, NICK, USER, JOIN, QUIT } id;

	explicit Command();
	explicit Command(const std::string &input);
};
