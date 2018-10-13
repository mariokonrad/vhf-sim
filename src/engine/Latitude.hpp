// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__ENGINE__LATITUDE__HPP__
#define __VHFSIM__ENGINE__LATITUDE__HPP__

#include "Angle.hpp"
#include "Parser.hpp"
#include <cmath>
#include <cstdio>

namespace vhfsim
{
namespace engine
{
class Latitude : public Angle
{
private:
	class Parser : public ::vhfsim::engine::Parser
	{
	public:
		typedef value_t Result;

	private:
		Result & result;

	public:
		Parser(Scanner & scan, Result & result)
			: ::vhfsim::engine::Parser(scan)
			, result(result)
		{
		}

		virtual void parse() { angle(); }
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
			}
			hemisphere();
		}

		// degree := digit_09 digit_09 .
		// range: 00..90
		void degree()
		{
			Scanner::size_type p0 = scan.pos();
			digit_09();
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			sscanf(scan.sub(p0, scan.pos()).c_str(), "%3u", &result.d);
			range(result.d, 0, 90);
		}

		// hemisphere := 'N' | 'S' .
		void hemisphere()
		{
			char c = scan.get();
			if (c != 'S' && c != 'N')
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
	Latitude(char = 'N', unsigned int = 0, unsigned int = 0, unsigned int = 0);
	virtual ~Latitude() {}
	virtual std::string str() const override;
	virtual void str(char *) const override;
	static bool parse(Latitude &, const std::string &);
};
}
}

#endif
