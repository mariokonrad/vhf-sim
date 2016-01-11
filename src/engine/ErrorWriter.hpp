// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ENGINE__ERRORWRITER__HPP__
#define __SIMRADRD68__ENGINE__ERRORWRITER__HPP__

#include <string>

namespace simradrd68
{
namespace engine
{
class ErrorWriter
{
public:
	virtual ~ErrorWriter() {}
	virtual void engine_error(const std::string &) = 0;
};
}
}

#endif
