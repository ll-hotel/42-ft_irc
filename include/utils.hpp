#pragma once

#include <string>
#include <vector>

bool patternMatch(const std::string &pat, const std::string &str);
bool patternMatch(const std::vector<std::string> &patterns, const std::string &str);

std::string strTolower(std::string str);
std::string strToupper(std::string str);

bool str_isprint(const std::string &str);
bool isvalid_nick(const std::string &str);

bool rfc_is_special(char c);
bool rfc_is_letter(char c);
bool rfc_is_number(char c);
