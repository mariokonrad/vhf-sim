// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "Date.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>

namespace simradrd68
{
namespace engine
{
Date::Date()
{
	t.year = 2000;
	t.month = 1;
	t.day = 1;
	t.hour = 0;
	t.min = 0;
	t.sec = 0;
}

Date::Date(const Date & other)
	: t(other.t)
{
}

Date::Date(unsigned short year, unsigned short month, unsigned short day, unsigned short hour,
	unsigned short min, unsigned short sec)
{
	t.year = year;
	t.month = month;
	t.day = day;
	t.hour = hour;
	t.min = min;
	t.sec = sec;
}

Date::Date(const Time & t)
	: t(t)
{
}

bool Date::operator==(const Date & other) const
{
	return true && t.year == other.t.year && t.month == other.t.month && t.day == other.t.day
		&& t.hour == other.t.hour && t.min == other.t.min && t.sec == other.t.sec;
}

bool Date::operator!=(const Date & other) const { return !(*this == other); }

Date & Date::operator=(const Date & other)
{
	t.year = other.t.year;
	t.month = other.t.month;
	t.day = other.t.day;
	t.hour = other.t.hour;
	t.min = other.t.min;
	t.sec = other.t.sec;
	return *this;
}

std::string Date::str() const
{
	char buf[32];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u", t.year, t.month, t.day, t.hour, t.min, t.sec);
	return buf;
}

void Date::str(char * buf) const
{
	assert(buf);
	std::string s = str();
	strncpy(buf, s.c_str(), s.size());
}

bool Date::parse(Date & d, const std::string & s)
{
	Time r;
	memset(&r, 0, sizeof(r));
	Scanner scan(s);
	Parser p(scan, r);
	try {
		p.parse();
		scan.skip_blank();
		if (!scan.eof())
			throw Parser::exception(scan, __FUNCTION__);
	} catch (Parser::exception &) {
		return false;
	}
	if (!valid(r))
		return false;
	d = Date(r);
	return true;
}

bool Date::valid(const Time & t)
{
	if (t.year != 0) {
		static const unsigned int DIM[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		if (t.month == 0 || t.month > 12)
			return false;
		if (t.day == 0 || t.day > DIM[t.month - 1])
			return false;
	}
	if (t.hour >= 24)
		return false;
	if (t.min >= 60)
		return false;
	if (t.sec >= 60)
		return false;
	return true;
}
}
}
