// Copyright (c) 2007-2010 Mario Konrad
// All Rights reserved.

#include "Rectangle.hpp"
#include <QPainter>

namespace simradrd68
{
Rectangle::Rectangle(float x0, float y0, float x1, float y1)
	: x0(x0)
	, y0(y0)
	, x1(x1)
	, y1(y1)
{
}

Rectangle::~Rectangle() {}

bool Rectangle::within(float x, float y) const
{
	return (x >= x0) && (x <= x1) && (y >= y0) && (y <= y1);
}

void Rectangle::draw(QPainter & painter) const { painter.drawRect(x0, y0, x1 - x0, y1 - y0); }
}
