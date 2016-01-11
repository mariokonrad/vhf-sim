// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include <cstdlib>
#include <cstdio>
#include "nmea.hpp"

namespace nmea
{
struct type_t {
	const char * id;
	Type t;
};

static const type_t TYPE[] = {
	{"$GPRMC", TYPE_RMC},
};

bool parse(Field & f, const std::string & s, std::string::size_type pos)
{
	using std::string;
	string::size_type j = pos;
	string::size_type l = s.size();
	if (j >= l)
		return false;
	while (j < l) {
		string::value_type c = s[j];
		if (c == ',')
			break;
		if (c == '*')
			break;
		++j;
	}
	f = s.substr(pos, j - pos);
	return true;
}

void parse(Fields & v, const std::string & s)
{
	using std::string;
	string::size_type i = 0;
	string f;
	while (parse(f, s, i)) {
		i += f.size() + 1;
		v.push_back(f);
	}
}

unsigned char checksum(const std::string & s)
{
	unsigned char r = 0;
	for (std::string::const_iterator i = s.begin(); i != s.end(); ++i) {
		const std::string::value_type c = *i;
		if (c == '$')
			continue;
		if (c == '*')
			break;
		r ^= c;
	}
	return r;
}

bool check(const std::string & s)
{
	Fields f;
	parse(f, s);
	if (f.size() < 2)
		return false;
	char buf[4];
	sprintf(buf, "%02X", checksum(s));
	return f.back() == buf;
}

Type type(const std::string & s)
{
	std::string f;
	if (parse(f, s) && f.size()) {
		for (unsigned int i = 0; i < sizeof(TYPE) / sizeof(type_t); ++i)
			if (s == TYPE[i].id)
				return TYPE[i].t;
	}
	return TYPE_UNKNOWN;
}

Type type(const Fields & v)
{
	if (!v.empty())
		return type(v[0]);
	return TYPE_UNKNOWN;
}
}
