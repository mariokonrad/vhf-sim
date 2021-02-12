// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef NMEA_NMEA_SENTENCE_HPP
#define NMEA_NMEA_SENTENCE_HPP

#include "nmea.hpp"
#include "engine/Date.hpp"
#include "engine/Latitude.hpp"
#include "engine/Longitude.hpp"

namespace nmea
{
class RMC
{
public:
	std::string time; // "hhmmss"
	char status;
	float lat; // not to be calculated with this!
	char lat_h;
	float lon; // not to be calculated with this!
	char lon_h;
	float v;
	float heading;
	std::string date; // "ddmmyy"
	float m;
	char m_h;
	char mode;

public:
	RMC();
	static bool set(RMC & d, const Fields &);
	bool set(const Fields &);
	std::string str() const;
	void set(const vhfsim::engine::Date &);
	void set(const vhfsim::engine::Latitude &);
	void set(const vhfsim::engine::Longitude &);
	bool get(vhfsim::engine::Date &);
	bool get(vhfsim::engine::Latitude &);
	bool get(vhfsim::engine::Longitude &);
};
}

#endif
