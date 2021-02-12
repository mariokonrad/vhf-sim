// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_MMSI_HPP
#define VHFSIM_ENGINE_MMSI_HPP

#include <string>

namespace vhfsim
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
