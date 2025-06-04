#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "reply_info.hpp"
#include <string>

static std::string buildNumericReplyBase(const NumericReplyCode &code, const std::string &host,
										 const User &user)
{
	return ':' + host + ' ' + ft_ltoa(code) + ' ' + user.nickname + ' ';
}

void Server::errNoSuchChannel(User &user, const std::string &channel) const
{
	user.send(buildNumericReplyBase(ERR_NOSUCHCHANNEL, m_hostname, user) + channel +
			  " :" ERR_NOSUCHCHANNEL_MESSAGE "\r\n");
}

void Server::errNotOnChannel(User &user, const std::string &channel) const
{
	user.send(buildNumericReplyBase(ERR_NOTONCHANNEL, m_hostname, user) + channel +
			  " :" ERR_NOTONCHANNEL_MESSAGE "\r\n");
}

void Server::errNeedMoreParams(User &user, const std::string &cmd) const
{
	user.send(buildNumericReplyBase(ERR_NEEDMOREPARAMS, m_hostname, user) + cmd +
			  " :" ERR_NEEDMOREPARAMS_MESSAGE "\r\n");
}

void Server::errChanOPrivsNeeded(User &user, const std::string &channel) const
{
	user.send(buildNumericReplyBase(ERR_CHANOPRIVSNEEDED, m_hostname, user) + channel +
			  " :" ERR_CHANOPRIVSNEEDED_MESSAGE "\r\n");
}

void Server::errUserNotInChannel(User &user, const std::string &nick,
								 const std::string &channel) const
{
	user.send(buildNumericReplyBase(ERR_USERNOTINCHANNEL, m_hostname, user) + nick + ' ' + channel +
			  " :" ERR_USERNOTINCHANNEL_MESSAGE "\r\n");
}

void Server::errAlreadyRegistered(User &user) const
{
	user.send(buildNumericReplyBase(ERR_ALREADYREGISTERED, m_hostname, user) +
			  " :" ERR_ALREADYREGISTERED_MESSAGE "\r\n");
}

void Server::errNoRecipient(User &user, const std::string &cmd) const
{
	user.send(buildNumericReplyBase(ERR_NORECIPIENT, m_hostname, user) +
			  " :" ERR_NORECIPIENT_MESSAGE ": " + cmd + "\r\n");
}

void Server::errNoTextToSend(User &user) const
{
	user.send(buildNumericReplyBase(ERR_NOTEXTTOSEND, m_hostname, user) +
			  " :" ERR_NOTEXTTOSEND_MESSAGE "\r\n");
}

void Server::errNoSuchNick(User &user, const std::string &nick) const
{
	user.send(buildNumericReplyBase(ERR_NOSUCHNICK, m_hostname, user) + nick +
			  " :" ERR_NOSUCHNICK_MESSAGE "\r\n");
}

void Server::errNicknameInUse(User &user, const std::string &nick) const
{
	user.send(buildNumericReplyBase(ERR_NICKNAMEINUSE, m_hostname, user) + nick +
			  " :" ERR_NICKNAMEINUSE_MESSAGE "\r\n");
}

void Server::errNickCollision(User &user, const std::string &nick) const
{
	user.send(buildNumericReplyBase(ERR_NICKCOLLISION, m_hostname, user) + nick +
			  " :" ERR_NICKCOLLISION_MESSAGE "\r\n");
}

void Server::rplNamReply(User &user, const char symbol, const std::string &channel,
						 const std::vector<std::string> &nicks) const
{
	std::string reply =
		buildNumericReplyBase(RPL_NAMREPLY, m_hostname, user) + symbol + ' ' + channel + " :";

	for (std::vector<std::string>::const_iterator nick = nicks.begin(); nick != nicks.end();
		 nick++) {
		reply.append(*nick + ' ');
	}
	if (not nicks.empty())
		reply.erase(reply.end() - 1);
	reply.append("\r\n");
	user.send(reply);
}

void Server::rplEndOfNames(User &user, const std::string &channel) const
{
	user.send(buildNumericReplyBase(RPL_ENDOFNAMES, m_hostname, user) + channel +
			  " :" RPL_ENDOFNAMES_MESSAGE "\r\n");
}

void Server::rplEndOfWho(User &user, const std::string &chan) const
{
	user.send(buildNumericReplyBase(RPL_ENDOFWHO, m_hostname, user) + chan + " :" +
			  "End of WHO list" + "\r\n");
}

void Server::rplWelcome(User &user) const
{
	user.send(buildNumericReplyBase(RPL_WELCOME, m_hostname, user) + ":" RPL_WELCOME_MESSAGE
																	 "\r\n");
}

void Server::errKeySet(User &user, const std::string &channel_name) const
{
	std::string msg = this->getReplyBase(ERR_KEYSET, user);
	msg.append(" ");
	msg.append(channel_name);
	msg.append(" :Channel key already set");
	msg.append("\r\n");
	user.send(msg);
}

void Server::errChannelIsFull(User &user, const std::string &channel_name) const
{
	std::string msg = this->getReplyBase(ERR_CHANNELISFULL, user);
	msg.append(" ");
	msg.append(channel_name);
	msg.append(" :Cannot join channel (+l)");
	msg.append("\r\n");
	user.send(msg);
}

void Server::errBadChannelKey(User &user, const std::string &channel_name) const
{
	std::string msg = this->getReplyBase(ERR_BADCHANNELKEY, user);

	msg.append(" ");
	msg.append(channel_name);
	msg.append(" :Cannot join channel (+k)");
	msg.append("\r\n");
	user.send(msg);
}

void Server::errInviteOnlyChan(User &user, const std::string &channel_name) const
{
	std::string msg = this->getReplyBase(ERR_INVITEONLYCHAN, user);

	msg.append(" ");
	msg.append(channel_name);
	msg.append(" :Cannot join channel (+i)");
	msg.append("\r\n");
	user.send(msg);
}

void Server::rplChannelModeIs(User &user, const Channel &channel) const
{
	std::string flag = "+";
	std::string args;
	std::string msg = this->getReplyBase(RPL_CHANNELMODEIS, user) + " ";

	if (channel.password_set) {
		flag += "k";
		if (channel.users.find(user.id) == channel.users.end()) {
			args.append(channel.password.size(), '*');
		}
		else
			args.append(channel.password);
		args.append(" ");
	}
	if (channel.limit_user != (size_t)(-1)) {
		flag += "l";
		args += ft_ltoa(channel.limit_user);
		args.append(" ");
	}
	if (channel.topic_op_only) {
		flag += "t";
	}
	if (channel.invite_only) {
		flag += "i";
	}
	msg += channel.name() + " " + (flag.size() != 1 ? (flag + " " + args) : "");
	user.send(msg + "\r\n");
}

void Server::errUnknownMode(User &user, const std::string &channel_name, std::string c) const
{
	std::string msg = this->getReplyBase(ERR_UNKNOWNMODE, user);

	msg.append(" ");
	msg.append(c);
	msg.append(" :is unknown mode char to me for ");
	msg.append(channel_name);
	msg.append("\r\n");
	user.send(msg);
}

void Server::rplNoTopic(User &user, const std::string &channel) const
{
	user.send(buildNumericReplyBase(RPL_NOTOPIC, m_hostname, user) + channel +
			  " :" RPL_NOTOPIC_MESSAGE "\r\n");
}

void Server::rplTopic(User &user, const std::string &channel, const std::string &topic) const
{
	user.send(buildNumericReplyBase(RPL_TOPIC, m_hostname, user) + channel + " :" + topic + "\r\n");
}
