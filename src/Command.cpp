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
	if (command_name == "CAP") {
		return Command::CAP;
	}
	else if (command_name == "PASS") {
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
	else if (command_name == "QUIT") {
		return Command::QUIT;
	}
	return Command::UNKNOWN;
}

static void parse_args(const std::string &input, size_t pos, std::vector<std::string> &args)
{
	while (pos < input.length()) {
		while (pos < input.length() and input[pos] == ' ')
			pos += 1;
		if (pos == input.length())
			break;
		size_t word_end;
		if (input[pos] == ':') {
			word_end = std::string::npos;
			pos += 1;
		}
		else {
			word_end = pos;
			while (word_end < input.length() and word_end != ' ')
				word_end += 1;
		}
		args.push_back(input.substr(pos, word_end));
		pos = word_end;
	}
}
