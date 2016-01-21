// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __VHFSIM__BUTTON__HPP__
#define __VHFSIM__BUTTON__HPP__

class QPainter;

namespace vhfsim
{
class Button
{
public:
	virtual ~Button() {}
	virtual bool within(float, float) const = 0;
	virtual void draw(QPainter &) const = 0;
};
}

#endif
