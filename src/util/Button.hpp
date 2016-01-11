// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef __SIMRADRD68__BUTTON__HPP__
#define __SIMRADRD68__BUTTON__HPP__

class QPainter;

namespace simradrd68
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
