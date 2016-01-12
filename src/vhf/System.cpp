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

	com.host = "localhost";
	com.port = 9540;

	gps.on_top = false;

	cc.mmsi = engine::MMSI("111111111");
	cc.group = engine::MMSI("033333333");
	cc.all_msg = false;
	cc.on_top = false;
}

System & System::inst()
{
	if (!instance)
		instance.reset(new System);
	assert(instance);
	return *instance;
}

bool System::load()
{
	System & sys = inst();
	QSettings * config = sys.config;
	if (!config)
		return false;
	config->setValue("global/type", sys.global.type.c_str());
	config->setValue("global/lang", sys.global.lang.c_str());
	config->setValue("global/exam_mode", sys.global.exam_mode);
	config->setValue("com/host", sys.com.host.c_str());
	config->setValue("com/port", sys.com.port);
	config->setValue("gps/lat", sys.gps.lat.str().c_str());
	config->setValue("gps/lon", sys.gps.lon.str().c_str());
	config->setValue("gps/time", sys.gps.time.str().c_str());
	config->setValue("gps/on_top", sys.gps.on_top);
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
	config->setValue("cc/on_top", sys.cc.on_top);
	config->beginWriteArray("dir");
	for (auto i = 0u; i < sys.dir.size(); ++i) {
		config->setArrayIndex(i);
		config->setValue("name", sys.dir[i].name.c_str());
		config->setValue("mmsi", sys.dir[i].mmsi.c_str());
	}
	config->endArray();
	config->sync();
	return true;
}

bool System::save()
{
	System & sys = inst();
	if (!sys.mod)
		return true;
	QSettings * config = sys.config;
	if (!config)
		return false;
	config->sync();
	sys.global.type = config->value("global/type").toString().toStdString();
	sys.global.lang = config->value("global/lang").toString().toStdString();
	sys.global.exam_mode = config->value("global/exam_mode").toBool();
	sys.com.host = config->value("com/host").toString().toStdString();
	sys.com.port = config->value("com/port").toInt();
	engine::Latitude::parse(sys.gps.lat, config->value("gps/lat").toString().toStdString());
	engine::Longitude::parse(sys.gps.lon, config->value("gps/lon").toString().toStdString());
	engine::Date::parse(sys.gps.time, config->value("gps/time").toString().toStdString());
	sys.gps.on_top = config->value("gps/on_top").toBool();
	sys.vhf.mmsi = engine::MMSI{config->value("vhf/mmsi").toString().toStdString()};
	sys.vhf.group = engine::MMSI{config->value("vhf/group").toString().toStdString()};
	engine::Date::parse(sys.vhf.time, config->value("vhf/time").toString().toStdString());
	engine::Latitude::parse(sys.vhf.lat, config->value("vhf/lat").toString().toStdString());
	engine::Longitude::parse(sys.vhf.lon, config->value("vhf/lon").toString().toStdString());
	sys.cc.mmsi = engine::MMSI{config->value("cc/mmsi").toString().toStdString()};
	sys.cc.group = engine::MMSI{config->value("cc/group").toString().toStdString()};
	engine::Latitude::parse(sys.cc.lat, config->value("cc/lat").toString().toStdString());
	engine::Longitude::parse(sys.cc.lon, config->value("cc/lon").toString().toStdString());
	engine::Date::parse(sys.cc.time, config->value("cc/time").toString().toStdString());
	sys.cc.all_msg = config->value("cc/all_msg").toBool();
	sys.cc.on_top = config->value("cc/on_top").toBool();
	auto dir_entries = config->beginReadArray("dir");
	for (auto i = 0; i < dir_entries; ++i) {
		config->setArrayIndex(i);
		std::string name = config->value("name").toString().toStdString();
		std::string mmsi = config->value("mmsi").toString().toStdString();
		sys.dir.emplace_back(name, mmsi);
	}
	config->endArray();
	sys.mod = false;
	return true;
}
}
