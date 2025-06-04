#include "Command.hpp"
#include <string>

Command::Command() : name(), args(), id(UNKNOWN)
{
}

static Command::Id find_id(const std::string &command_name) throw();
static void parse_args(const std::string &input, size_t pos, std::vector<std::string> &args);

Command::Command(const std::string &input) : name(), args(), id(UNKNOWN)
{
	name = input.substr(0, input.find(' '));
	this->id = find_id(this->name);
	if (this->id == Command::UNKNOWN)
		return;
	parse_args(input, this->name.length(), this->args);
}

static Command::Id find_id(const std::string &command_name) throw()
{
	if (command_name == "PASS") {
		return Command::PASS;
	}
	else if (command_name == "NICK") {
		return Command::NICK;
	}
	else if (command_name == "USER") {
		return Command::USER;
	}
	else if (command_name == "JOIN") {
		return Command::JOIN;
	}
	else if (command_name == "TOPIC") {
		return Command::TOPIC;
	}
	else if (command_name == "PART") {
		return Command::PART;
	}
	else if (command_name == "PRIVMSG") {
		return Command::PRIVMSG;
	}
	else if (command_name == "NAMES") {
		return Command::NAMES;
	}
	else if (command_name == "KICK") {
		return Command::KICK;
	}
	else if (command_name == "QUIT") {
		return Command::QUIT;
	}
	else if (command_name == "INVITE") {
		return Command::INVITE;
	}
	else if (command_name == "MODE") {
		return Command::MODE;
	}
	else if (command_name == "MOTD") {
		return Command::MOTD;
	}
	else if (command_name == "PING") {
		return Command::PING;
	}
	return Command::UNKNOWN;
}

static void parse_args(const std::string &input, size_t pos, std::vector<std::string> &args)
{
	while (pos < input.length()) {
		pos = input.find_first_not_of(' ', pos);
		if (pos >= input.length())
			break;
		size_t word_end;
		if (input[pos] == ':') {
			word_end = std::string::npos;
			pos += 1;
		}
		else {
			word_end = input.find_first_of(' ', pos);
		}
		args.push_back(input.substr(pos, word_end - pos));
		pos = word_end;
	}
}

std::ostream &operator<<(std::ostream &os, const Command &command)
{
	os << command.name << ':';
	for (std::vector<std::string>::const_iterator it = command.args.begin();
		 it != command.args.end(); it += 1) {
		os << ' ' << '"' << *it << '"';
	}
	return os;
}
