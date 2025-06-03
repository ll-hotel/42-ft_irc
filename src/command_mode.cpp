#include "Server.hpp"
#include <cstdlib>
#include <queue>
#include <string>
#include <vector>

typedef std::vector<std::string> str_vec;

typedef std::pair<std::string, str_vec> cmd_type;

struct s_mode
{
	std::queue<cmd_type> cmds;
	std::string errChar;
};

std::vector<cmd_type> exec_mode(s_mode &mode, User &user, Channel &channel, Server &server);

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
				if (expected_param_numbers == -1) {
					mode.errChar = str[m];
					return (ERR_UNKNOWNMODE);
				}
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
			mode.errChar = str[0];
			return ERR_UNKNOWNMODE;
		}
	}
	return RET_ZERO;
}

void Server::commandMode(const Command &command, User &user)
{
	struct s_mode mode;
	NumericReplyCode err;
	std::vector<Channel *>::iterator channel;

	if (command.args.size() == 0) {
		this->errNeedMoreParams(user, command.name);
		return;
	}
	channel = getChannelByName(command.args[0]);
	if (channel == m_channels.end()) {
		this->errNotOnChannel(user, command.args[0]);
		return;
	}
	if (command.args.size() == 1) {
		this->rplChannelModeIs(user, **channel);
		return;
	}
	if (!isUserOp(user, **channel)) {
		this->errChanOPrivsNeeded(user, command.args[0]);
		// not_op ERR_CHANOPRIVSNEEDED
		return;
	}
	err = parse_modes(command.args, mode);
	if (err != RET_ZERO) {
		if (err == ERR_NEEDMOREPARAMS)
			this->errNeedMoreParams(user, command.name);
		else if (err == ERR_UNKNOWNMODE)
			this->errUnknownMode(user, (*channel)->name(), mode.errChar);
		// bad_parsing
		return;
	}
	std::vector<cmd_type> valide_cmds;
	valide_cmds = exec_mode(mode, user, **channel, *this);
	std::string msg = ":" + user.clientName(this->m_hostname) + " MODE " + (*channel)->name() + " ";
	for (size_t cur = 0; cur < valide_cmds.size(); cur++) {
		std::string reply_msg = valide_cmds[cur].first;
		for (size_t i = 0; i < valide_cmds[cur].second.size(); i++)
			reply_msg += " " + valide_cmds[cur].second[i];
		reply_msg.append("\r\n");
		(*channel)->broadcast(msg + reply_msg);
	}
}

static bool is_number(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (str[i] < '0' || str[i] > '9')
			return (false);
	return (true);
}

std::vector<cmd_type> exec_mode(s_mode &mode, User &user, Channel &channel, Server &server)
{
	std::vector<cmd_type> valide_cmds;

	for (size_t cur = 0, end = (mode.cmds.size() > 3 ? 3 : mode.cmds.size()); cur < end; cur++) {
		cmd_type cmd = mode.cmds.front();
		switch (cmd.first[1]) {
		case 'l': {
			if (cmd.first[0] == '-')
				channel.limit_user = -1;
			if (cmd.first[0] == '+') {
				if (!is_number(cmd.second[0]))
					break;
				channel.limit_user = std::atol(cmd.second[0].c_str());
			}
			valide_cmds.push_back(cmd);
		} break;
		case 'i': {
			if (cmd.first[0] == '-')
				channel.invite_only = false;
			else if (cmd.first[0] == '+')
				channel.invite_only = true;
			valide_cmds.push_back(cmd);
		} break;
		case 't': {
			if (cmd.first[0] == '-')
				channel.topic_op_only = false;
			else if (cmd.first[0] == '+')
				channel.topic_op_only = true;
			valide_cmds.push_back(cmd);
		} break;
		case 'o': {
			std::vector<User *>::const_iterator user_it = server.getUserbyNickname(cmd.second[0]);
			if (user_it == server.getUsers().end()) {
				server.errUserNotInChannel(user, cmd.second[0], channel.name());
				break;
			}
			if (channel.users.find((*user_it)->id) == channel.users.end()) {
				server.errUserNotInChannel(user, cmd.second[0], channel.name());
				break;
			}
			if (cmd.first[0] == '-')
				channel.ops.erase((*user_it)->id);
			else if (cmd.first[0] == '+')
				channel.ops.insert((*user_it)->id);
			valide_cmds.push_back(cmd);
		} break;
		case 'k': {
			if (cmd.first[0] == '-') {
				if (channel.password_set && cmd.second[0] == channel.password) {
					channel.password_set = false;
				}
				else {
					break;
				}
			}
			else if (cmd.first[0] == '+') {
				if (channel.password_set) {
					server.errKeySet(user, channel.name());
					break;
				}
				else {
					channel.password = cmd.second[0];
					channel.password_set = true;
				}
			}
			valide_cmds.push_back(cmd);
		} break;
		default:
			break;
		}
		mode.cmds.pop();
	}
	return valide_cmds;
}
