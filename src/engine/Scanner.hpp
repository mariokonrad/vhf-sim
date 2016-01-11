// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ENGINE__SCANNER__HPP__
#define __SIMRADRD68__ENGINE__SCANNER__HPP__

#include <string>

namespace simradrd68
{
namespace engine
{

class Scanner
{
public:
	typedef std::string::size_type size_type;
	enum { NPOS = std::string::npos };

private:
	std::string s;
	size_type p;

public:
	Scanner(const std::string & s)
		: s(s)
		, p(std::string::npos)
	{
		reset();
	}

	char peek(size_type n = 1) const
	{
		size_type size = s.size();
		if (size < n)
			return '\0';
		if (p >= size - n)
			return '\0';
		return (p != std::string::npos) ? s[p + n] : '\0';
	}

	void skip_blank()
	{
		size_type size = s.size();
		while ((p < size) && isspace(s[p]))
			++p;
		if (p >= size)
			p = std::string::npos;
	}

	void advance(size_type n = 1, bool skip = false)
	{
		size_type size = s.size();
		while (n > 0) {
			if (p < size) {
				++p;
				--n;
			} else {
				p = std::string::npos;
				n = 0;
			}
		}
		if (skip)
			skip_blank();
	}

	inline void reset() { p = (s.size() > 0) ? 0 : std::string::npos; }

	inline bool eof() const { return (p == std::string::npos) || (p == s.size()); }

	inline char get() const { return peek(0); }
	inline size_type pos() const { return p; }

	inline std::string sub(size_type p0, size_type p1) { return s.substr(p0, p1 - p0); }
};
}
}

#endif
