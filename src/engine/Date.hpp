// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_DATE_HPP
#define VHFSIM_ENGINE_DATE_HPP

#include <string>
#include <cstdlib>
#include "Parser.hpp"

namespace vhfsim
{
namespace engine
{
class Date
{
private:
	struct Time {
		unsigned short year;
		unsigned short month;
		unsigned short day;
		unsigned short hour;
		unsigned short min;
		unsigned short sec;
	};

private:
	class Parser : public ::vhfsim::engine::Parser
	{
	private:
		Time & result;

	public:
		Parser(Scanner & scan, Time & result)
			: ::vhfsim::engine::Parser(scan)
			, result(result)
		{
		}

		virtual void parse() { timestamp(); }
	private:
		// timestamp := [ date space ] time .
		void timestamp()
		{
			char p0 = scan.peek(0);
			char p1 = scan.peek(1);
			char p2 = scan.peek(2);
			if (is_digit_09(p0) && is_digit_09(p1) && is_digit_09(p2)) {
				date();
				space();
			}
			time();
		}

		// date := year minus month minus day .
		void date()
		{
			year();
			minus();
			month();
			minus();
			day();
		}

		// time := hour colon minute [ colon second ] .
		void time()
		{
			hour();
			colon();
			minute();
			if (is_colon(scan.get())) {
				colon();
				second();
			}
		}

		// year := digit_09 digit_09 digit_09 digit_09 .
		// range: 0000..9999
		void year()
		{
			Scanner::size_type p0 = scan.pos();
			digit_09();
			digit_09();
			digit_09();
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			result.year = strtoul(scan.sub(p0, scan.pos()).c_str(), 0, 10);
			range(result.year, 0, 9999);
		}

		// month := ('0' | '1') digit_09 .
		// range: 01..12
		void month()
		{
			Scanner::size_type p0 = scan.pos();
			one_of("01");
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			result.month = strtoul(scan.sub(p0, scan.pos()).c_str(), 0, 10);
			range(result.month, 1, 12);
		}

		// day := ('0' | '1' | '2' | '3') digit_09 .
		// range: 01..31
		void day()
		{
			Scanner::size_type p0 = scan.pos();
			one_of("0123");
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			result.day = strtoul(scan.sub(p0, scan.pos()).c_str(), 0, 10);
			range(result.day, 1, 31);
		}

		// hour := ('0' | '1' | '2') digit_09 .
		// range: 00..23
		void hour()
		{
			Scanner::size_type p0 = scan.pos();
			one_of("012");
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			result.hour = strtoul(scan.sub(p0, scan.pos()).c_str(), 0, 10);
			range(result.hour, 0, 23);
		}

		// hour := ('0' | '1' | '2' | '3' | '4' | '5') digit_09 .
		// range: 00..59
		void minute()
		{
			Scanner::size_type p0 = scan.pos();
			one_of("012345");
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			result.min = strtoul(scan.sub(p0, scan.pos()).c_str(), 0, 10);
			range(result.min, 0, 59);
		}

		// hour := ('0' | '1' | '2' | '3' | '4' | '5') digit_09 .
		// range: 00..59
		void second()
		{
			Scanner::size_type p0 = scan.pos();
			one_of("012345");
			digit_09();
			if (is_digit_09(scan.get()))
				throw exception(scan, __FUNCTION__);
			result.sec = strtoul(scan.sub(p0, scan.pos()).c_str(), 0, 10);
			range(result.sec, 0, 59);
		}
	};

private:
	Time t;

private:
	Date(const Time &);
	static bool valid(const Time &);

public:
	Date();
	Date(const Date &);
	Date(unsigned short, unsigned short, unsigned short, unsigned short, unsigned short,
		unsigned short);
	bool operator==(const Date &) const;
	bool operator!=(const Date &) const;
	Date & operator=(const Date &);
	std::string str() const;
	void str(char *) const;
	inline unsigned short year() const { return t.year; }
	inline unsigned short month() const { return t.month; }
	inline unsigned short day() const { return t.day; }
	inline unsigned short hour() const { return t.hour; }
	inline unsigned short min() const { return t.min; }
	inline unsigned short sec() const { return t.sec; }

	static bool parse(Date &, const std::string &);
};
}
}

#endif
