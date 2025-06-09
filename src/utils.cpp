
#include <string>

std::string strToupper(std::string str)
{
	for (size_t i = 0; i < str.size(); i++) {
		str[i] = std::toupper(str[i]);
	}
	return str;
}

std::string strTolower(std::string str)
{
	for (size_t i = 0; i < str.size(); i++) {
		str[i] = std::tolower(str[i]);
	}
	return str;
}
