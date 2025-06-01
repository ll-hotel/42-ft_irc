#pragma once

#define ERR_CHANOPRIVSNEEDED_MESSAGE "You are not a channel operator"
#define ERR_NOTONCHANNEL_MESSAGE "You are not on that channel"
#define ERR_NOSUCHCHANNEL_MESSAGE "No such channel"
#define ERR_NOSUCHNICK_MESSAGE "No such nickname"
#define ERR_NORECIPIENT_MESSAGE "No recipient given"
#define ERR_NOTEXTTOSEND_MESSAGE "No text to send"
#define ERR_NEEDMOREPARAMS_MESSAGE "Need more params"
#define ERR_USERNOTINCHANNEL_MESSAGE "User not in channel"
#define ERR_ALREADYREGISTERED_MESSAGE "Already registered"
#define ERR_NICKNAMEINUSE_MESSAGE "Nickname already in use"
#define ERR_NICKCOLLISION_MESSAGE "Nickname collision"
#define RPL_ENDOFNAMES_MESSAGE "End of /NAMES list"
#define RPL_NOTOPIC_MESSAGE "No topic is set"
#define RPL_WELCOME_MESSAGE "Welcome to our IRC server!"

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
	RPL_ENDOFNAMES = 366,
	ERR_NOTONCHANNEL = 442,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOSUCHCHANNEL = 403,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NOSUCHNICK = 401,
	ERR_USERNOTINCHANNEL = 441
};
