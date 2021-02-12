// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_DIRECTORY_HPP
#define VHFSIM_ENGINE_DIRECTORY_HPP

#include <string>
#include <vector>

namespace vhfsim
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
