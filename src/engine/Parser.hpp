// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_PARSER_HPP
#define VHFSIM_ENGINE_PARSER_HPP

#include "Scanner.hpp"
#include <limits.h>

namespace vhfsim
{
namespace engine
{
class Parser
{
public:
	class exception
	{
	private:
		Scanner::size_type p;
		const char * s;

	public:
		exception(Scanner::size_type pos, const char * s = nullptr)
			: p(pos)
			, s(s)
		{
		}

		exception(const Scanner & scan, const char * s = nullptr)
			: p(scan.pos())
			, s(s)
		{
		}

		inline Scanner::size_type pos() const { return p; }
		inline const char * what() const { return s; }
	};

protected:
	Scanner & scan;

protected:
	inline bool is_digit_09(char c) const { return (c >= '0') && (c <= '9'); }

	inline bool is_digit_07(char c) const { return (c >= '0') && (c <= '7'); }

	inline bool is_digit_19(char c) const { return (c >= '1') && (c <= '9'); }

	inline bool is_hex_digit(char c) const { return is_one_of(c, "0123456789ABCDEFabcdef"); }

	inline bool is_minus(char c) const { return c == '-'; }
	inline bool is_plus(char c) const { return c == '+'; }
	inline bool is_star(char c) const { return c == '*'; }
	inline bool is_slash(char c) const { return c == '/'; }
	inline bool is_colon(char c) const { return c == ':'; }
	inline bool is_semicolon(char c) const { return c == ';'; }
	inline bool is_point(char c) const { return c == '.'; }
	inline bool is_space(char c) const { return c == ' '; }

	inline bool is_alpha(char c) const
	{
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
	}

	inline bool is_alphanum(char c) const { return is_alpha(c) || is_digit_09(c); }

	inline bool is_one_of(char c, const char * s) const
	{
		while (*s)
			if (*s == c)
				return true;
			else
				++s;
		return false;
	}

public:
	Parser(Scanner & scan)
		: scan(scan)
	{
	}

	virtual ~Parser() {}

	virtual void parse() = 0;

protected:
	inline void range(int val, int lo = INT_MIN, int hi = INT_MAX) const
	{
		if (val < lo || val > hi)
			throw exception(scan, __FUNCTION__);
	}

	inline void range(double val, double lo, double hi, double eps = 1.0e-5) const
	{
		if (val < lo - eps || val > hi + eps)
			throw exception(scan, __FUNCTION__);
	}

	inline void one_of(const char * s)
	{
		if (!is_one_of(scan.get(), s))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void digit_09()
	{
		if (!is_digit_09(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void digit_19()
	{
		if (!is_digit_19(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void digit_07()
	{
		if (!is_digit_07(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void hex_digit()
	{
		if (!is_hex_digit(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void colon()
	{
		if (!is_colon(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void point()
	{
		if (!is_point(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void space()
	{
		if (!is_space(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void minus()
	{
		if (!is_minus(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}

	inline void slash()
	{
		if (!is_slash(scan.get()))
			throw exception(scan, __FUNCTION__);
		scan.advance();
	}
};
}
}

#endif
