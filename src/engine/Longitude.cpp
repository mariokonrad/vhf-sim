// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "Longitude.hpp"
#include <cmath>
#include <cassert>
#include <cstring>

namespace vhfsim
{
namespace engine
{
Longitude::Longitude(char h, unsigned int d, unsigned int m, unsigned int s)
{
	val.h = h;
	val.d = d;
	val.m = m;
	val.s = s;
	if (!valid(val)) {
		val.h = 'W';
		val.d = 0;
		val.m = 0;
		val.s = 0;
	}
}

bool Longitude::valid(const value_t & v)
{
	if ((v.d > 180) || (v.m >= 60) || (v.s >= 60))
		return false;
	if ((v.d == 180) && ((v.m > 0) || (v.s > 0)))
		return false;
	if ((v.h != 'E') && (v.h != 'W'))
		return false;
	return true;
}

std::string Longitude::str() const
{
	char buf[16];
	sprintf(buf, "%03u-%02u%c", deg(), min(), hem());
	return buf;
}

void Longitude::str(char * buf) const
{
	assert(buf);
	std::string s = str();
	strncpy(buf, s.c_str(), s.size());
}

bool Longitude::parse(Longitude & l, const std::string & str)
{
	Parser::Result result;
	memset(&result, 0, sizeof(result));
	Scanner s(str);
	Parser p(s, result);

	try {
		p.parse();
		s.skip_blank();
		if (!s.eof())
			throw Parser::exception(s, "EOF");
	} catch (Parser::exception & e) {
		return false;
	}
	if (!valid(result))
		return false;
	l = Longitude(result.h, result.d, result.m, result.s);
	return true;
}
}
}
