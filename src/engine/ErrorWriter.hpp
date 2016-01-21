// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__ENGINE__ERRORWRITER__HPP__
#define __VHFSIM__ENGINE__ERRORWRITER__HPP__

#include <string>

namespace vhfsim
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
