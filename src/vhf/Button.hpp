// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_BUTTON_HPP
#define VHFSIM_BUTTON_HPP

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
