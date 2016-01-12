// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__SYSTEM__HPP__
#define __SIMRADRD68__SYSTEM__HPP__

#include <string>
#include <memory>
#include "engine/MMSI.hpp"
#include "engine/Latitude.hpp"
#include "engine/Longitude.hpp"
#include "engine/Directory.hpp"
#include "engine/Date.hpp"

class QSettings;

namespace simradrd68
{
class System
{
private:
	System();
	System(const System &) = delete;
	System(System &&) = delete;
	System & operator=(const System &) = delete;
	System & operator=(System &&) = delete;

private:
	static std::unique_ptr<System> instance;

private:
	bool mod;
	bool _lat;
	bool _lon;
	bool _time;
	struct {
		std::string type;
		std::string lang;
		bool exam_mode;
	} global;
	struct {
		std::string host;
		short port;
	} com;
	struct {
		engine::Date time;
		engine::Latitude lat;
		engine::Longitude lon;
		bool on_top;
	} gps;
	struct {
		engine::MMSI mmsi;
		engine::MMSI group;
		engine::Date time;
		engine::Latitude lat;
		engine::Longitude lon;
	} vhf;
	struct {
		engine::MMSI mmsi;
		engine::MMSI group;
		engine::Date time;
		engine::Latitude lat;
		engine::Longitude lon;
		bool all_msg;
		bool on_top;
	} cc;
	engine::Directory dir;
	QSettings * config;
	bool sound_status;

private:
	static System & inst();

public:
	static void init(QSettings * config) { inst().config = config; }
	static bool load();
	static bool save();

	static bool modified() { return inst().mod; }
	static bool vhf_lat_set() { return inst()._lat; }
	static bool vhf_lon_set() { return inst()._lon; }
	static bool vhf_time_set() { return inst()._time; }

	static void vhf_clear_pos_time()
	{
		System & sys = inst();
		sys._time = false;
		sys._lat = false;
		sys._lon = false;
	}

	// sound ------------------------------------------

	static void sound_enable(bool flag) { inst().sound_status = flag; }
	static bool sound_enabled() { return inst().sound_status; }
	static bool sound_disabled() { return !inst().sound_status; }

	// channels ---------------------------------------

	// global -----------------------------------------
	static void lang(const std::string & lang)
	{
		System & sys = inst();
		if (sys.global.lang != lang) {
			sys.global.lang = lang;
			sys.mod = true;
		}
	}

	static void exam_mode(bool f)
	{
		System & sys = inst();
		if (sys.global.exam_mode != f)
			sys.mod = true;
		sys.global.exam_mode = f;
	}

	static std::string lang() { return inst().global.lang; }

	static std::string type() { return inst().global.type; }
	static bool exam_mode() { return inst().global.exam_mode; }

	// communication ----------------------------------
	static void com_host(const std::string & s)
	{
		System & sys = inst();
		if (sys.com.host != s)
			sys.mod = true;
		sys.com.host = s;
	}

	static void com_port(short p)
	{
		System & sys = inst();
		if (sys.com.port != p)
			sys.mod = true;
		sys.com.port = p;
	}

	static std::string com_host() { return inst().com.host; }
	static short com_port() { return inst().com.port; }

	// gps --------------------------------------------
	static void gps_latitude(const engine::Latitude & l)
	{
		System & sys = inst();
		if (sys.gps.lat != l)
			sys.mod = true;
		sys.gps.lat = l;
	}

	static void gps_longitude(const engine::Longitude & l)
	{
		System & sys = inst();
		if (sys.gps.lon != l)
			sys.mod = true;
		sys.gps.lon = l;
	}

	static void gps_time(const engine::Date & t)
	{
		System & sys = inst();
		if (sys.gps.time != engine::Date(t))
			sys.mod = true;
		sys.gps.time = t;
	}

	static void gps_on_top(bool f)
	{
		System & sys = inst();
		if (sys.gps.on_top != f)
			sys.mod = true;
		sys.gps.on_top = f;
	}

	static engine::Latitude gps_latitude() { return inst().gps.lat; }
	static engine::Longitude gps_longitude() { return inst().gps.lon; }
	static engine::Date gps_time() { return inst().gps.time; }
	static bool gps_on_top() { return inst().gps.on_top; }

	// dir --------------------------------------------
	static void dir_set(const engine::Directory & dir)
	{
		System & sys = inst();
		sys.dir = dir;
		sys.mod = true;
	}

	static engine::Directory dir_get() { return inst().dir; }

	// vhf --------------------------------------------
	static void vhf_mmsi(const engine::MMSI & mmsi)
	{
		System & sys = inst();
		if (sys.vhf.mmsi != mmsi)
			sys.mod = true;
		sys.vhf.mmsi = mmsi;
	}

	static void vhf_group(const engine::MMSI & mmsi)
	{
		System & sys = inst();
		if (sys.vhf.group != mmsi)
			sys.mod = true;
		sys.vhf.group = mmsi;
	}

	static void vhf_latitude(const engine::Latitude & l)
	{
		System & sys = inst();
		if (sys.vhf.lat != l)
			sys.mod = true;
		sys._lat = true;
		sys.vhf.lat = l;
	}

	static void vhf_longitude(const engine::Longitude & l)
	{
		System & sys = inst();
		if (sys.vhf.lon != l)
			sys.mod = true;
		sys._lon = true;
		sys.vhf.lon = l;
	}

	static void vhf_time(const engine::Date & t)
	{
		System & sys = inst();
		if (sys.vhf.time != engine::Date(t))
			sys.mod = true;
		sys._time = true;
		sys.vhf.time = t;
	}

	static engine::MMSI vhf_mmsi() { return inst().vhf.mmsi; }
	static engine::MMSI vhf_group() { return inst().vhf.group; }
	static engine::Latitude vhf_latitude() { return inst().vhf.lat; }
	static engine::Longitude vhf_longitude() { return inst().vhf.lon; }
	static engine::Date vhf_time() { return inst().vhf.time; }

	// control center ---------------------------------
	static void cc_mmsi(const engine::MMSI & mmsi)
	{
		System & sys = inst();
		if (sys.cc.mmsi != mmsi)
			sys.mod = true;
		sys.cc.mmsi = mmsi;
	}

	static void cc_group(const engine::MMSI & mmsi)
	{
		System & sys = inst();
		if (sys.cc.group != mmsi)
			sys.mod = true;
		sys.cc.group = mmsi;
	}

	static void cc_latitude(const engine::Latitude & l)
	{
		System & sys = inst();
		if (sys.cc.lat != l)
			sys.mod = true;
		sys.cc.lat = l;
	}

	static void cc_longitude(const engine::Longitude & l)
	{
		System & sys = inst();
		if (sys.cc.lon != l)
			sys.mod = true;
		sys.cc.lon = l;
	}

	static void cc_time(const engine::Date & t)
	{
		System & sys = inst();
		if (sys.cc.time != engine::Date(t))
			sys.mod = true;
		sys.cc.time = t;
	}

	static void cc_all_msg(bool flag)
	{
		System & sys = inst();
		if (sys.cc.all_msg != flag)
			sys.mod = true;
		sys.cc.all_msg = flag;
	}

	static void cc_on_top(bool f)
	{
		System & sys = inst();
		if (sys.cc.on_top != f)
			sys.mod = true;
		sys.cc.on_top = f;
	}

	static engine::MMSI cc_mmsi() { return inst().cc.mmsi; }
	static engine::MMSI cc_group() { return inst().cc.group; }
	static engine::Latitude cc_latitude() { return inst().cc.lat; }
	static engine::Longitude cc_longitude() { return inst().cc.lon; }
	static engine::Date cc_time() { return inst().cc.time; }
	static bool cc_all_msg() { return inst().cc.all_msg; }
	static bool cc_on_top() { return inst().cc.on_top; }
};
}

#endif
