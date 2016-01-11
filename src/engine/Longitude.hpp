// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ENGINE__LONGITUDE__HPP__
#define __SIMRADRD68__ENGINE__LONGITUDE__HPP__

#include "Angle.hpp"
#include "Parser.hpp"
#include <cmath>
#include <cstdio>

namespace simradrd68
{
namespace engine
{

class Longitude : public Angle
{
private:
	class Parser : public ::simradrd68::engine::Parser
	{
	public:
		typedef value_t Result;

	private:
		Result & result;

	public:
		Parser(Scanner & scan, Result & result)
			: ::simradrd68::engine::Parser(scan)
			, result(result)
		{
		}

		virtual void parse() throw(Parser::exception) { angle(); }
	private:
		// angle := degree [ minus minute [ point second_rational ] ] hemisphere .
		void angle()
		{
			degree();
			if (is_minus(scan.get())) {
				minus();
				minute();
				if (is_point(scan.get())) {
					point();
					second_rational();
				}
				hemisphere();
			}
		}

		// degree := digit_09 digit_09 digit_09 .
		// range: 00..180
		void degree()
		{
			Scanner::size_type p0 = scan.pos();
			digit_09();
			digit_09();
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			sscanf(scan.sub(p0, scan.pos()).c_str(), "%5u", &result.d);
			range(result.d, 0, 180);
		}

		// hemisphere := 'E' | 'W' .
		void hemisphere()
		{
			char c = scan.get();
			if (c != 'W' && c != 'E')
				throw exception(scan, __FUNCTION__);
			scan.advance();
			result.h = c;
		}

		// minute := digit_09 digit_09 .
		// range: 00..59
		void minute()
		{
			Scanner::size_type p0 = scan.pos();
			digit_09();
			digit_09();
			sscanf(scan.sub(p0, scan.pos()).c_str(), "%3u", &result.m);
			range(result.m, 0, 59);
		}

		// second_rational := digit_09 .
		// range: 0..9
		void second_rational()
		{
			Scanner::size_type p0 = scan.pos();
			digit_09();
			sscanf(scan.sub(p0, scan.pos()).c_str(), "%3u", &result.s);
			range(result.s, 0, 9);
			result.s *= static_cast<unsigned int>(floor(60.0 / 100.0 + 0.5));
		}
	};

private:
	static bool valid(const value_t &);

public:
	Longitude(char = 'E', unsigned int = 0, unsigned int = 0, unsigned int = 0);
	virtual std::string str() const;
	virtual void str(char *) const;
	static bool parse(Longitude &, const std::string &);
};
}
}

#endif
