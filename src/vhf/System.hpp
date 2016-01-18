// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__SYSTEM__HPP__
#define __SIMRADRD68__SYSTEM__HPP__

#include <string>
#include <memory>
#include <QString>
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
		QString type;
		QString lang;
		bool exam_mode;
	} global;
	struct {
		QString host;
		short port;
	} com;
	struct {
		engine::Date time;
		engine::Latitude lat;
		engine::Longitude lon;
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

	static bool modified();
	static bool vhf_lat_set();
	static bool vhf_lon_set();
	static bool vhf_time_set();
	static void vhf_clear_pos_time();

	// sound ------------------------------------------

	static void sound_enable(bool flag);
	static bool sound_enabled();
	static bool sound_disabled();

	// channels ---------------------------------------

	// global -----------------------------------------
	static void lang(const QString & lang);
	static QString lang();

	static void exam_mode(bool f);
	static bool exam_mode();

	static QString type();

	// communication ----------------------------------
	static void com_host(const QString & s);
	static QString com_host();

	static void com_port(short p);
	static short com_port();

	// gps --------------------------------------------
	static void gps_latitude(const engine::Latitude & l);
	static engine::Latitude gps_latitude();

	static void gps_longitude(const engine::Longitude & l);
	static engine::Longitude gps_longitude();

	static void gps_time(const engine::Date & t);
	static engine::Date gps_time();

	// dir --------------------------------------------
	static void dir_set(const engine::Directory & dir);
	static engine::Directory dir_get();

	// vhf --------------------------------------------
	static void vhf_mmsi(const engine::MMSI & mmsi);
	static engine::MMSI vhf_mmsi();

	static void vhf_group(const engine::MMSI & mmsi);
	static engine::MMSI vhf_group();

	static void vhf_latitude(const engine::Latitude & l);
	static engine::Latitude vhf_latitude();

	static void vhf_longitude(const engine::Longitude & l);
	static engine::Longitude vhf_longitude();

	static void vhf_time(const engine::Date & t);
	static engine::Date vhf_time();

	// control center ---------------------------------
	static void cc_mmsi(const engine::MMSI & mmsi);
	static engine::MMSI cc_mmsi();

	static void cc_group(const engine::MMSI & mmsi);
	static engine::MMSI cc_group();

	static void cc_latitude(const engine::Latitude & l);
	static engine::Latitude cc_latitude();

	static void cc_longitude(const engine::Longitude & l);
	static engine::Longitude cc_longitude();

	static void cc_time(const engine::Date & t);
	static engine::Date cc_time();

	static void cc_all_msg(bool flag);
	static bool cc_all_msg();
};
}

#endif
