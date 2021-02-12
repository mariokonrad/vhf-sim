// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#ifndef VHFSIM_CIRCLE_HPP
#define VHFSIM_CIRCLE_HPP

#include "Button.hpp"

namespace vhfsim
{
class Circle : public Button
{
private:
	float m_x;
	float m_y;
	float r;

public:
	Circle(float, float, float);
	virtual ~Circle();
	virtual bool within(float, float) const override;
	virtual void draw(QPainter &) const override;
};
}

#endif
