#include "Server.hpp"
#include <queue>
#include <string>
#include <vector>

typedef std::pair<std::string, std::vector<std::string>> cmd_type;

struct s_mode
{
	std::queue<cmd_type> cmds;
};

int param_number(char c, char s)
{
	switch (c) {
	case 'k':
		return 1;
	case 'o':
		return 1;
	case 'l':
		return s == '+';
	case 't':
		return 0;
	case 'i':
		return 0;
	default:
		return -1;
	}
}

NumericReplyCode parse_modes(const std::vector<std::string> args, s_mode &mode)
{
	for (size_t c = 1; c < args.size(); c++) {
		const std::string &str = args[c];
		if (str.size() == 0)
			continue;
		if (str[0] == '+' || str[0] == '-') {
			std::string s = std::string() + str[0];
			for (size_t m = 1; m < str.size(); m++) {
				int expected_param_numbers = param_number(str[m], str[0]);
				if (expected_param_numbers == -1)
					return (ERR_UNKNOWNMODE);
				std::vector<std::string> param;
				while (expected_param_numbers--) {
					if (c + 1 >= args.size())
						return (ERR_NEEDMOREPARAMS);
					param.push_back(args[++c]);
				}
				mode.cmds.push(cmd_type(s + str[m], param));
			}
		}
		else {
			return ERR_UNKNOWNMODE;
		}
	}
	return RET_ZERO;
}

#include <iostream>

int main()
{
	s_mode mode;
	std::vector<std::string> args;
	args.push_back("command_name");
	args.push_back("+kk");
	args.push_back("password");
	args.push_back("password2");
	args.push_back("+l");
	args.push_back("password3");
	args.push_back("-l");
	std::cout << parse_modes(args, mode) << std::endl;
	while (mode.cmds.size()) {
		cmd_type &cmd = mode.cmds.front();
		std::cout << "opt: " << cmd.first << std::endl;
		std::cout << "param:" << std::endl;
		for (size_t i = 0; i < cmd.second.size(); i++)
			std::cout << "\t" << cmd.second[i] << std::endl;
		mode.cmds.pop();
	}
}

void Server::commandMode(const Command &command, User &user)
{
	struct s_mode mode;
	NumericReplyCode err;
	std::vector<Channel *>::iterator channel = getChannelByName(command.args[0]);

	if (channel == m_channels.end()) {
		// no_channel
		return;
	}
	if (!isUserOp(user, **channel)) {
		// not_op
		return;
	}
	err = parse_modes(command.args, mode);
	if (err != RET_ZERO) {
		// bad_parsing
		return;
	}
	if (mode.cmds.size() > 3) {
		// more_than_3_cmd
		return;
	}
	// exec_cmd(mode);
}
