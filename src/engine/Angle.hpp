// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__ENGINE__ANGLE__HPP__
#define __SIMRADRD68__ENGINE__ANGLE__HPP__

#include <string>

namespace simradrd68
{
namespace engine
{
class Angle
{
protected:
	struct value_t {
		char h;
		unsigned int d;
		unsigned int m;
		unsigned int s;
	} val;

public:
	Angle();
	Angle(const Angle &);
	virtual ~Angle() {}

	Angle & operator=(const Angle &);
	bool operator==(const Angle &) const;
	bool operator!=(const Angle &) const;

	inline unsigned int deg() const { return val.d; }
	inline unsigned int min() const { return val.m; }
	inline unsigned int sec() const { return val.s; }
	inline char hem() const { return val.h; }

	virtual std::string str() const = 0;
	virtual void str(char *) const = 0;
};
}
}

#endif
