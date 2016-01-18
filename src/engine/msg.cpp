// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "msg.hpp"
#include "endian.hpp"
#include <cstdio>
#include <cstring>

namespace simradrd68
{
namespace engine
{
msg_t::msg_t() { memset(this, 0, sizeof(msg_t)); }

void msg_dump(const msg_t & m)
{
	printf("type    = 0x%04x\n", m.type);
	printf("lat     = [%s]\n", m.lat);
	printf("lon     = [%s]\n", m.lon);
	printf("time    = [%s]\n", m.time);
	printf("mmsi    = [%s]\n", m.mmsi);
	printf("group   = [%s]\n", m.group);
	printf("ch      = %d\n", m.channel);
	printf("work_ch = %d\n", m.work_channel);
	printf("pwr     = %d\n", m.power);
	printf("DSC     = { type=%04x, recv=%04x, desig=%d, mmsi=[%s] }\n", m.dsc.type, m.dsc.recv,
		m.dsc.designation, m.dsc.mmsi);
}

msg_t hton(const msg_t & m)
{
	msg_t r = m;
	r.type = endian::hton(r.type);
	r.channel = endian::hton(r.channel);
	r.work_channel = endian::hton(r.work_channel);
	r.power = endian::hton(r.power);
	r.dsc.type = endian::hton(r.dsc.type);
	r.dsc.recv = endian::hton(r.dsc.recv);
	r.dsc.designation = endian::hton(r.dsc.designation);
	r.valid_lat = endian::hton(r.valid_lat);
	r.valid_lon = endian::hton(r.valid_lon);
	r.valid_time = endian::hton(r.valid_time);
	return r;
}

msg_t ntoh(const msg_t & m)
{
	msg_t r = m;
	r.type = endian::ntoh(r.type);
	r.channel = endian::ntoh(r.channel);
	r.work_channel = endian::ntoh(r.work_channel);
	r.power = endian::ntoh(r.power);
	r.dsc.type = endian::ntoh(r.dsc.type);
	r.dsc.recv = endian::ntoh(r.dsc.recv);
	r.dsc.designation = endian::ntoh(r.dsc.designation);
	r.valid_lat = endian::ntoh(r.valid_lat);
	r.valid_lon = endian::ntoh(r.valid_lon);
	r.valid_time = endian::ntoh(r.valid_time);
	return r;
}

void msg_set_latitude(msg_t & m, const std::string & s)
{
	m.valid_lat = 1;
	strncpy(m.lat, s.c_str(), sizeof(m.lat));
}

void msg_set_longitude(msg_t & m, const std::string & s)
{
	m.valid_lon = 1;
	strncpy(m.lon, s.c_str(), sizeof(m.lon));
}
}
}
