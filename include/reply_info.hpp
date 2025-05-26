#pragma once

#define ERR_CHANOPRIVSNEEDED_MESSAGE "You're not channel operator"
#define ERR_NOTONCHANNEL_MESSAGE "You're not on that channel"
#define ERR_NOSUCHCHANNEL_MESSAGE "No such channel"
#define RPL_NOTOPIC_MESSAGE "No topic is set"

enum NumericReplyCode {
	RPL_WELCOME = 1,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_NONICKNAMEGIVEN = 431,
	// ERR_ERRONEUSNICKNAME = 432, /* Not used, because we do not have a nickname 'blacklist' */
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	RPL_NAMREPLY = 353,
	ERR_NOTONCHANNEL = 442,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOSUCHCHANNEL = 403
};
