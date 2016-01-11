// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ENGINE__DIRECTORY__HPP__
#define __SIMRADRD68__ENGINE__DIRECTORY__HPP__

#include <string>
#include <vector>

namespace simradrd68
{
namespace engine
{
class DirEntry
{
public:
	std::string name;
	std::string mmsi;

public:
	DirEntry()
		: name("")
		, mmsi("")
	{
	}
	DirEntry(const std::string & name, const std::string & mmsi)
		: name(name)
		, mmsi(mmsi)
	{
	}
};

typedef std::vector<DirEntry> Directory;
}
}

#endif
