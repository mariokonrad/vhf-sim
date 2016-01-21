// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "MMSI.hpp"
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>

namespace vhfsim
{
namespace engine
{
MMSI::MMSI() { memset(&num, 0, sizeof(num)); }

MMSI::MMSI(const char * num) { set(strtoul(num, 0, 10)); }

MMSI::MMSI(const std::string & num) { set(strtoul(num.c_str(), 0, 10)); }

MMSI::MMSI(unsigned long num) { set(num); }

MMSI::MMSI(const MMSI & other) { memcpy(&num, &other.num, sizeof(num)); }

void MMSI::set(unsigned long num)
{
	this->num.id = num % 1000000;
	this->num.mid = num / 1000000;
}

MMSI & MMSI::operator=(const MMSI & other)
{
	memcpy(&num, &other.num, sizeof(num));
	return *this;
}

bool MMSI::operator==(const MMSI & other) const
{
	return memcmp(&num, &other.num, sizeof(num)) == 0;
}

bool MMSI::operator!=(const MMSI & other) const
{
	return memcmp(&num, &other.num, sizeof(num)) != 0;
}

std::string MMSI::str() const
{
	char buf[10];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%03lu%06lu", num.mid, num.id);
	return std::string(buf);
}

void MMSI::str(char * buf) const
{
	assert(buf);
	std::string s = str();
	strncpy(buf, s.c_str(), s.size());
}

bool MMSI::is_coast_station() const { return num.mid >= 1 && num.mid <= 9; }

bool MMSI::is_group_station() const { return num.mid >= 10 && num.mid <= 99; }
}
}
