// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ENGINE__MMSI__HPP__
#define __SIMRADRD68__ENGINE__MMSI__HPP__

#include <string>

namespace simradrd68
{
namespace engine
{
class MMSI
{
private:
	struct Number {
		unsigned long mid;
		unsigned long id;
	} num;

private:
	void set(unsigned long);

public:
	MMSI();
	MMSI(const char *);
	MMSI(const std::string &);
	MMSI(unsigned long);
	MMSI(const MMSI &);
	MMSI & operator=(const MMSI &);
	bool operator==(const MMSI &) const;
	bool operator!=(const MMSI &) const;
	std::string str() const;
	void str(char *) const;

	bool is_coast_station() const;
	bool is_group_station() const;
};
}
}

#endif
