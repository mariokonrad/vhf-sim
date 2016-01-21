// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__ENGINE__MODEL__HPP__
#define __VHFSIM__ENGINE__MODEL__HPP__

#include "MMSI.hpp"
#include "Latitude.hpp"
#include "Longitude.hpp"
#include "Date.hpp"
#include "Directory.hpp"

namespace vhfsim
{
namespace engine
{
class Model
{
public:
	virtual ~Model() {}

	// general
	virtual bool vhf_lat_set() = 0;
	virtual bool vhf_lon_set() = 0;
	virtual bool vhf_time_set() = 0;
	virtual void vhf_clear_pos_time() = 0;

	// MMSI
	virtual MMSI mmsi() = 0;
	virtual void mmsi(const MMSI &) = 0;
	virtual MMSI group() = 0;
	virtual void group(const MMSI &) = 0;

	// position
	virtual void lat(const Latitude &) = 0;
	virtual Latitude lat() = 0;
	virtual void lon(const Longitude &) = 0;
	virtual Longitude lon() = 0;

	// time
	virtual void time(const Date &) = 0;
	virtual Date time() = 0;

	// directory
	virtual void dir_set(const Directory &) = 0;
	virtual Directory dir_get() = 0;
};
}
}

#endif
