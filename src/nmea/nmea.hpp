// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __NMEA__NMEA__HPP__
#define __NMEA__NMEA__HPP__

#include <string>
#include <vector>

namespace nmea
{
enum Type { TYPE_UNKNOWN = 0, TYPE_RMC };

typedef std::string Field;
typedef std::vector<Field> Fields;

bool parse(Field &, const std::string &, std::string::size_type = 0);
void parse(Fields &, const std::string &);
unsigned char checksum(const std::string &);
bool check(const std::string &);
Type type(const std::string &);
Type type(const Fields &);
}

#endif
