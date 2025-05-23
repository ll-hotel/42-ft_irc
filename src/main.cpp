#include <Server.hpp>
#include <cstdlib>
#include <iostream>

int error(const std::string &exec_file_name, int cause);
bool is_number(const std::string &str);

int main(int argc, char **argv)
{
	if (argc != 3)
		return (error(argv[0], 0));
	if (!is_number(argv[1]))
		return (error(argv[0], 1));

	Server server(atol(argv[1]), argv[2]);
	server.run();
	return (0);
}

bool is_number(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (str[i] < '0' || str[i] > '9')
			return (false);
	return (true);
}

int error(const std::string &exec_file_name, int cause)
{
	if (cause == 0)
		std::cerr << "bad command format:" << std::endl;
	else if (cause == 1)
		std::cerr << "'port' need to be a number:" << std::endl;
	std::cerr << "\t" + exec_file_name + " <port> <password>" << std::endl;
	return (1);
}
