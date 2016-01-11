// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "Angle.hpp"
#include <cstring>

namespace simradrd68
{
namespace engine
{
Angle::Angle() { memset(&val, 0, sizeof(val)); }

Angle::Angle(const Angle & other)
	: val(other.val)
{
}

Angle & Angle::operator=(const Angle & other)
{
	val = other.val;
	return *this;
}

bool Angle::operator==(const Angle & other) const
{
	return memcmp(&val, &other.val, sizeof(val)) == 0;
}

bool Angle::operator!=(const Angle & other) const
{
	return memcmp(&val, &other.val, sizeof(val)) != 0;
}
}
}
