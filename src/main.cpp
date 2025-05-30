#include <Server.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>

static int error(const std::string &exec_file_name, int cause);
static bool is_number(const std::string &str);
static void sigint_handler(int);

bool sigint_received = false;

int main(int argc, char **argv)
try {
	std::signal(SIGINT, sigint_handler);

	if (argc != 3)
		return (error(argv[0], 0));
	if (!is_number(argv[1]))
		return (error(argv[0], 1));

	Server server(atol(argv[1]), argv[2]);
	server.run();
	return (0);
} catch (const std::exception &e) {
	std::cerr << e.what() << std::endl;
	return 1;
}

static bool is_number(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (str[i] < '0' || str[i] > '9')
			return (false);
	return (true);
}

static int error(const std::string &exec_file_name, int cause)
{
	if (cause == 0)
		std::cerr << "bad command format:" << std::endl;
	else if (cause == 1)
		std::cerr << "'port' need to be a number:" << std::endl;
	std::cerr << "\t" + exec_file_name + " <port> <password>" << std::endl;
	return (1);
}

static void sigint_handler(int)
{
	sigint_received = true;
}
