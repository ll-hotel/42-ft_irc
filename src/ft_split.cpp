#include "ft_split.hpp"

std::vector<std::string> ft_split(const std::string &str, const char sep)
{
	std::vector<std::string> pieces;
	size_t pos = 0;
	size_t piece_end = 0;
	for (pos = 0; pos < str.size(); pos = piece_end) {
		while (pos < str.size() and str[pos] == sep)
			pos += 1;
		if (pos == str.size())
			break;
		const size_t piece_pos = pos;
		while (pos < str.size() and str[pos] != sep)
			pos += 1;
		piece_end = pos;
		pieces.push_back(str.substr(piece_pos, piece_end - piece_pos));
	}
	return pieces;
}
