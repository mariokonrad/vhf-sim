// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "nmea_sentence.hpp"
#include <cmath>
#include <cstring>

namespace nmea
{
static inline void field(std::string & f, const Field & v) { f = v; }

static inline void field(char & f, const Field & v, char def = '\0')
{
	f = v.empty() ? def : v[0];
}

static inline void field(float & f, const Field & v, float def = 0.0f)
{
	f = v.empty() ? def : atof(v.c_str());
}

static inline void field(double & f, const Field & v, double def = 0.0)
{
	f = v.empty() ? def : atof(v.c_str());
}

RMC::RMC()
	: time("")
	, status('A')
	, lat(0.0)
	, lat_h('N')
	, lon(0.0)
	, lon_h('E')
	, v(0.0)
	, heading(0.0)
	, date("")
	, m(0.0)
	, m_h('E')
	, mode('A')
{
}

std::string RMC::str() const
{
	char buf[128];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "$GPRMC,%s,%c,%07.3f,%c,%08.3f,%c,%04.1f,%04.1f,%s,%04.1f,%c,%c*",
		time.c_str(), status, lat, lat_h, lon, lon_h, v, heading, date.c_str(), m, m_h, mode);
	char scs[4];
	memset(scs, 0, sizeof(scs));
	sprintf(scs, "%02X", checksum(buf));
	return std::string(buf) + std::string(scs);
}

bool RMC::set(RMC & d, const Fields & f)
{
	if (type(f) != TYPE_RMC)
		return false;
	if (f.size() != 14)
		return false;
	field(d.time, f[1]);
	field(d.status, f[2]);
	field(d.lat, f[3]);
	field(d.lat_h, f[4]);
	field(d.lon, f[5]);
	field(d.lon_h, f[6]);
	field(d.v, f[7]);
	field(d.heading, f[8]);
	field(d.date, f[9]);
	field(d.m, f[10]);
	field(d.m_h, f[11]);
	field(d.mode, f[12]);
	return true;
}

bool RMC::set(const Fields & f) { return set(*this, f); }

void RMC::set(const vhfsim::engine::Date & d)
{
	char buf[8];

	// set time
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%02d%02d%02d", d.hour(), d.min(), d.sec());
	time = buf;

	// set date
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%02d%02d%02d", d.day(), d.month(), d.year() % 100);
	date = buf;
}

void RMC::set(const vhfsim::engine::Latitude & l)
{
	lat = static_cast<float>(l.deg()) * 100.0f + l.min();
	lat_h = l.hem();
}

void RMC::set(const vhfsim::engine::Longitude & l)
{
	lon = static_cast<float>(l.deg()) * 100.0f + l.min();
	lon_h = l.hem();
}

bool RMC::get(vhfsim::engine::Date & d)
{
	unsigned long vd = strtoul(date.c_str(), 0, 10); // "ddmmyy"
	unsigned long vt = strtoul(time.c_str(), 0, 10); // "hhmmss"
	d = vhfsim::engine::Date((vd % 100) + 2000, (vd / 100) % 100, (vd / 10000) % 100,
		(vt / 10000) % 100, (vt / 100) % 100, (vt % 100));
	return true;
}

bool RMC::get(vhfsim::engine::Latitude & l)
{
	unsigned int deg = static_cast<unsigned int>(floor(lat / 100.0));
	unsigned int min = static_cast<unsigned int>(floor(fmod(lat, 100.0)));
	l = vhfsim::engine::Latitude(lat_h, deg, min); // ignoring seconds
	return true;
}

bool RMC::get(vhfsim::engine::Longitude & l)
{
	unsigned int deg = static_cast<unsigned int>(floor(lon / 100.0));
	unsigned int min = static_cast<unsigned int>(floor(fmod(lon, 100.0)));
	l = vhfsim::engine::Longitude(lon_h, deg, min); // ignoring seconds
	return true;
}
}
