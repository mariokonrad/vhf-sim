// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_ENGINE_ERRORWRITER_HPP
#define VHFSIM_ENGINE_ERRORWRITER_HPP

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
