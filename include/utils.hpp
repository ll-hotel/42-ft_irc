#pragma once

#include <string>
#include <vector>

bool patternMatch(const std::string &pat, const std::string &str);
bool patternMatch(const std::vector<std::string> &patterns, const std::string &str);

std::string strTolower(std::string str);
std::string strToupper(std::string str);