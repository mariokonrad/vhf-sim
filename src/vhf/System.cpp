// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include <cassert>
#include <QSettings>
#include "System.hpp"

namespace simradrd68
{
std::unique_ptr<System> System::instance;

System::System()
	: mod(false)
	, _lat(false)
	, _lon(false)
	, _time(false)
	, config(nullptr)
	, sound_status(true)
{
	// default values

	global.type = "simrad-rd68";
	global.lang = "en_US";
	global.exam_mode = false;

	vhf_fix.mmsi_readonly = false;

	com.host = "localhost";
	com.port = 9540;

	cc.mmsi = engine::MMSI("111111111");
	cc.group = engine::MMSI("033333333");
	cc.all_msg = false;
}

System & System::inst()
{
	if (!instance)
		instance.reset(new System);
	assert(instance);
	return *instance;
}

void System::init(QSettings * config)
{
	inst().config = config;
	inst().load();
}

void System::load()
{
	if (!config)
		return;
	if (!config->isWritable())
		return;

	global.type = config->value("global/type", global.type).toString();
	global.lang = config->value("global/lang", global.lang).toString();
	global.exam_mode = config->value("global/exam_mode", global.exam_mode).toBool();
	com.host = config->value("com/host", com.host).toString();
	com.port = config->value("com/port", com.port).toInt();
	engine::Latitude::parse(gps.lat, config->value("gps/lat").toString().toStdString());
	engine::Longitude::parse(gps.lon, config->value("gps/lon").toString().toStdString());
	engine::Date::parse(gps.time, config->value("gps/time").toString().toStdString());
	vhf.mmsi = engine::MMSI{config->value("vhf/mmsi").toString().toStdString()};
	vhf.group = engine::MMSI{config->value("vhf/group").toString().toStdString()};
	engine::Date::parse(vhf.time, config->value("vhf/time").toString().toStdString());
	engine::Latitude::parse(vhf.lat, config->value("vhf/lat").toString().toStdString());
	engine::Longitude::parse(vhf.lon, config->value("vhf/lon").toString().toStdString());
	cc.mmsi = engine::MMSI{config->value("cc/mmsi", cc.mmsi.str().c_str()).toString().toStdString()};
	cc.group = engine::MMSI{config->value("cc/group", cc.group.str().c_str()).toString().toStdString()};
	engine::Latitude::parse(cc.lat, config->value("cc/lat").toString().toStdString());
	engine::Longitude::parse(cc.lon, config->value("cc/lon").toString().toStdString());
	engine::Date::parse(cc.time, config->value("cc/time").toString().toStdString());
	cc.all_msg = config->value("cc/all_msg", cc.all_msg).toBool();
	auto dir_entries = config->beginReadArray("dir");
	for (auto i = 0; i < dir_entries; ++i) {
		config->setArrayIndex(i);
		std::string name = config->value("name").toString().toStdString();
		std::string mmsi = config->value("mmsi").toString().toStdString();
		dir.emplace_back(name, mmsi);
	}
	config->endArray();

	config->sync();
}

bool System::save()
{
	System & sys = inst();
	if (!sys.mod)
		return true;
	QSettings * config = sys.config;
	if (!config)
		return false;
	if (!config->isWritable())
		return false;
	config->sync();

	config->setValue("global/type", sys.global.type);
	config->setValue("global/lang", sys.global.lang);
	config->setValue("global/exam_mode", sys.global.exam_mode);
	config->setValue("com/host", sys.com.host);
	config->setValue("com/port", sys.com.port);
	config->setValue("gps/lat", sys.gps.lat.str().c_str());
	config->setValue("gps/lon", sys.gps.lon.str().c_str());
	config->setValue("gps/time", sys.gps.time.str().c_str());
	config->setValue("vhf/mmsi", sys.vhf.mmsi.str().c_str());
	config->setValue("vhf/group", sys.vhf.group.str().c_str());
	config->setValue("vhf/time", sys.vhf.time.str().c_str());
	config->setValue("vhf/lat", sys.vhf.lat.str().c_str());
	config->setValue("vhf/lon", sys.vhf.lon.str().c_str());
	config->setValue("cc/mmsi", sys.cc.mmsi.str().c_str());
	config->setValue("cc/group", sys.cc.group.str().c_str());
	config->setValue("cc/lat", sys.cc.lat.str().c_str());
	config->setValue("cc/lon", sys.cc.lon.str().c_str());
	config->setValue("cc/time", sys.cc.time.str().c_str());
	config->setValue("cc/all_msg", sys.cc.all_msg);
	config->beginWriteArray("dir");
	for (auto i = 0u; i < sys.dir.size(); ++i) {
		config->setArrayIndex(i);
		config->setValue("name", sys.dir[i].name.c_str());
		config->setValue("mmsi", sys.dir[i].mmsi.c_str());
	}
	config->endArray();

	sys.mod = false;
	return true;
}

bool System::vhf_lat_set() { return inst()._lat; }

bool System::vhf_lon_set() { return inst()._lon; }

bool System::vhf_time_set() { return inst()._time; }

void System::vhf_clear_pos_time()
{
	System & sys = inst();
	sys._time = false;
	sys._lat = false;
	sys._lon = false;
}

void System::sound_enable(bool flag) { inst().sound_status = flag; }

bool System::sound_enabled() { return inst().sound_status; }

bool System::sound_disabled() { return !inst().sound_status; }

void System::lang(const QString & lang)
{
	System & sys = inst();
	if (sys.global.lang != lang) {
		sys.global.lang = lang;
		sys.mod = true;
	}
}

QString System::lang() { return inst().global.lang; }

void System::exam_mode(bool f)
{
	System & sys = inst();
	if (sys.global.exam_mode != f)
		sys.mod = true;
	sys.global.exam_mode = f;
}

bool System::exam_mode() { return inst().global.exam_mode; }

QString System::type() { return inst().global.type; }

void System::com_host(const QString & s)
{
	System & sys = inst();
	if (sys.com.host != s)
		sys.mod = true;
	sys.com.host = s;
}

QString System::com_host() { return inst().com.host; }

void System::com_port(short p)
{
	System & sys = inst();
	if (sys.com.port != p)
		sys.mod = true;
	sys.com.port = p;
}

short System::com_port() { return inst().com.port; }

void System::gps_latitude(const engine::Latitude & l)
{
	System & sys = inst();
	if (sys.gps.lat != l)
		sys.mod = true;
	sys.gps.lat = l;
}

engine::Latitude System::gps_latitude() { return inst().gps.lat; }

void System::gps_longitude(const engine::Longitude & l)
{
	System & sys = inst();
	if (sys.gps.lon != l)
		sys.mod = true;
	sys.gps.lon = l;
}

engine::Longitude System::gps_longitude() { return inst().gps.lon; }

void System::gps_time(const engine::Date & t)
{
	System & sys = inst();
	if (sys.gps.time != engine::Date(t))
		sys.mod = true;
	sys.gps.time = t;
}

engine::Date System::gps_time() { return inst().gps.time; }

void System::dir_set(const engine::Directory & dir)
{
	System & sys = inst();
	sys.dir = dir;
	sys.mod = true;
}

engine::Directory System::dir_get() { return inst().dir; }

void System::vhf_mmsi(const engine::MMSI & mmsi)
{
	System & sys = inst();
	if (sys.vhf_fix.mmsi_readonly)
		return;
	if (sys.vhf.mmsi != mmsi)
		sys.mod = true;
	sys.vhf.mmsi = mmsi;
}

engine::MMSI System::vhf_mmsi()
{
	System & sys = inst();
	if (sys.vhf_fix.mmsi_readonly) {
		return sys.vhf_fix.mmsi;
	} else {
		return sys.vhf.mmsi;
	}
}

void System::fix_vhf_mmsi(const engine::MMSI & mmsi)
{
	System & sys = inst();
	sys.vhf_fix.mmsi_readonly = true;
	sys.vhf_fix.mmsi = mmsi;
}

bool System::is_vhf_mmsi_fix() { return inst().vhf_fix.mmsi_readonly; }

void System::vhf_group(const engine::MMSI & mmsi)
{
	System & sys = inst();
	if (sys.vhf.group != mmsi)
		sys.mod = true;
	sys.vhf.group = mmsi;
}

engine::MMSI System::vhf_group() { return inst().vhf.group; }

void System::vhf_latitude(const engine::Latitude & l)
{
	System & sys = inst();
	if (sys.vhf.lat != l)
		sys.mod = true;
	sys._lat = true;
	sys.vhf.lat = l;
}

engine::Latitude System::vhf_latitude() { return inst().vhf.lat; }

void System::vhf_longitude(const engine::Longitude & l)
{
	System & sys = inst();
	if (sys.vhf.lon != l)
		sys.mod = true;
	sys._lon = true;
	sys.vhf.lon = l;
}

engine::Longitude System::vhf_longitude() { return inst().vhf.lon; }

void System::vhf_time(const engine::Date & t)
{
	System & sys = inst();
	if (sys.vhf.time != engine::Date(t))
		sys.mod = true;
	sys._time = true;
	sys.vhf.time = t;
}

engine::Date System::vhf_time() { return inst().vhf.time; }

void System::cc_mmsi(const engine::MMSI & mmsi)
{
	System & sys = inst();
	if (sys.cc.mmsi != mmsi)
		sys.mod = true;
	sys.cc.mmsi = mmsi;
}

engine::MMSI System::cc_mmsi() { return inst().cc.mmsi; }

void System::cc_group(const engine::MMSI & mmsi)
{
	System & sys = inst();
	if (sys.cc.group != mmsi)
		sys.mod = true;
	sys.cc.group = mmsi;
}

engine::MMSI System::cc_group() { return inst().cc.group; }

void System::cc_latitude(const engine::Latitude & l)
{
	System & sys = inst();
	if (sys.cc.lat != l)
		sys.mod = true;
	sys.cc.lat = l;
}

engine::Latitude System::cc_latitude() { return inst().cc.lat; }

void System::cc_longitude(const engine::Longitude & l)
{
	System & sys = inst();
	if (sys.cc.lon != l)
		sys.mod = true;
	sys.cc.lon = l;
}

engine::Longitude System::cc_longitude() { return inst().cc.lon; }

void System::cc_time(const engine::Date & t)
{
	System & sys = inst();
	if (sys.cc.time != engine::Date(t))
		sys.mod = true;
	sys.cc.time = t;
}

engine::Date System::cc_time() { return inst().cc.time; }

void System::cc_all_msg(bool flag)
{
	System & sys = inst();
	if (sys.cc.all_msg != flag)
		sys.mod = true;
	sys.cc.all_msg = flag;
}

bool System::cc_all_msg() { return inst().cc.all_msg; }
}
