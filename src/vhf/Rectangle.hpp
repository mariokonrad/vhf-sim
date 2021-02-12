// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_RECTANGLE_HPP
#define VHFSIM_RECTANGLE_HPP

#include "Button.hpp"

namespace vhfsim
{
class Rectangle : public Button
{
private:
	float x0;
	float y0;
	float x1;
	float y1;

public:
	Rectangle(float, float, float, float);
	virtual ~Rectangle();
	virtual bool within(float, float) const override;
	virtual void draw(QPainter &) const override;
};
}

#endif
