// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __NMEA__NMEA_SENTENCE__HPP__
#define __NMEA__NMEA_SENTENCE__HPP__

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
	void set(const simradrd68::engine::Date &);
	void set(const simradrd68::engine::Latitude &);
	void set(const simradrd68::engine::Longitude &);
	bool get(simradrd68::engine::Date &);
	bool get(simradrd68::engine::Latitude &);
	bool get(simradrd68::engine::Longitude &);
};
}

#endif
