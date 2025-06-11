#include "utils.hpp"
#include <cctype>
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

bool str_isprint(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i += 1) {
		if (not std::isprint(str[i])) {
			return false;
		}
	}
	return true;
}

bool isvalid_nick(const std::string &str)
{
	if (str.empty() or not rfc_is_letter(str[0])) {
		return false;
	}
	for (size_t i = 1; i < str.size(); i += 1) {
		if (not(rfc_is_letter(str[i]) or rfc_is_number(str[i]) or rfc_is_special(str[i]))) {
			return false;
		}
	}
	return true;
}

bool rfc_is_special(char c)
{
	static const std::string set = "-[]\\`^{}";
	return set.find(c) != set.npos;
}

bool rfc_is_letter(char c)
{
	return std::isalpha(c);
}

bool rfc_is_number(char c)
{
	return std::isdigit(c);
}
