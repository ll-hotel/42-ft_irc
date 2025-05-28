#include <string>

static bool patternReq(const std::string &pat, const std::string &str, size_t pat_cur,
					   size_t str_cur)
{
	if (pat_cur == pat.size())
		return (str_cur == str.size());
	if (str_cur == str.size()) {
		while (pat[pat_cur] == '*')
			pat_cur++;
		return (pat_cur == pat.size());
	}
	if (pat[pat_cur] == str[str_cur] || pat[pat_cur] == '?') {
		while (pat_cur < pat.size() && (pat[pat_cur] == str[str_cur] || pat[pat_cur] == '?')) {
			pat_cur++;
			str_cur++;
		}
		return (patternReq(pat, str, pat_cur, str_cur));
	}
	if (pat[pat_cur] == '*')
		return (patternReq(pat, str, pat_cur, str_cur + 1) ||
				patternReq(pat, str, pat_cur + 1, str_cur));
	return (str_cur == str.size());
}

bool patternMatch(const std::string &pat, const std::string &str)
{
	return patternReq(pat, str, 0, 0);
}
