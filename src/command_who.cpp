#include "Server.hpp"

enum e_flag { WHO_O = 0X1 };

struct s_flags
{
	s_flags(int mask) : data(mask)
	{
	}
	int data;
};
std::vector<User *> list_all_users(s_flags flag, const std::vector<User *> &all_user);
std::vector<User *> list_user(s_flags flag, const std::string &mask,
							  const std::vector<User *> &all_users);
std::vector<User *> list_user_from_channels(s_flags flag, const std::string &mask,
											const std::vector<Channel *> &channels);

struct s_flags get_flags(const std::vector<std::string> &args);
std::vector<Channel *> get_channels(const std::string &mask,
									const std::vector<Channel *> &all_channels);

std::string generateResponse(const std::vector<User *> &user_list, const std::string &chanel,
							 const std::string &servername, User &client, const std::string &mask);

void Server::commandWho(const Command &command, User &user) const
{
	std::vector<User *> user_list;
	struct s_flags flags = get_flags(command.args);
	std::string mask;

	if (command.args.size() == 0)
		mask = "*";
	else
		mask = command.args[0];

	std::vector<Channel *> channels = get_channels(mask, this->m_channels);
	if (channels.size() == 0)
		user_list = list_user(flags, mask, this->m_users);
	else
		(void)""; // list_user_from_channels();

	{
		user_list = list_user(flags, command.args[0], this->m_users);
	}
	std::string response = generateResponse(user_list, "*", this->m_hostname, user, mask);
	user.send(response);
}

std::vector<User *> list_user(s_flags flag, const std::string &mask,
							  const std::vector<User *> &all_user)
{
	std::vector<User *> user_list;

	(void)flag;
	for (size_t i = 0; i < all_user.size(); i++) {
		if (!all_user[i]->registered())
			continue;
		if (all_user[i]->username == mask)
			user_list.push_back(all_user[i]);
	}
	return user_list;
}

struct s_flags get_flags(const std::vector<std::string> &args)
{
	struct s_flags flags(0);
	for (size_t i = 1; i < args.size(); i++) {
		for (size_t j = 0; j < args[i].size(); j++) {
			switch (args[i][j]) {
			case 'o':
				flags.data |= WHO_O;
				break;
			default:
				break;
			}
		}
	}
	return flags;
}

std::vector<Channel *> get_channels(const std::string &mask,
									const std::vector<Channel *> &all_channels)
{
	std::vector<Channel *> channels;
	for (size_t i = 0; i < all_channels.size(); i++) {
		if (all_channels[i]->name() == mask)
			channels.push_back(all_channels[i]);
	}
	return channels;
}

std::string generateResponse(const std::vector<User *> &user_list, const std::string &chanel,
							 const std::string &servername, User &client, const std::string &mask)
{
	std::string res;
	for (size_t i = 0; i < user_list.size(); i++) {
		User &u = *user_list[i];
		res += "352";
		res += client.nickname + " " + chanel + " " + u.username + " " + u.hostname + " " +
			   servername + " " + u.nickname + " " + "H" + " " + ": " + "0" + " " + u.realname;
		res += "\r\n";
	}
	res += "315 " + client.nickname + " " + mask + " : End of WHO list";
	return res;
}

std::vector<User *> list_all_users(s_flags flag, const std::vector<User *> &all_users)
{
	std::vector<User *> result;

	std::vector<User *>::const_iterator it = all_users.begin();
	std::vector<User *>::const_iterator it_end = all_users.end();
	(void)flag;
	for (; it != it_end; it++) {
		if (!(*it)->registered())
			continue;
		result.push_back(*it);
	}
	return result;
}
