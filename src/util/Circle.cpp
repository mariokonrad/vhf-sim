// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include "Circle.hpp"
#include <QPainter>

namespace simradrd68
{
template <typename T> static inline T sqr(const T & a) { return a * a; }

Circle::Circle(float x, float y, float r)
	: m_x(x)
	, m_y(y)
	, r(r)
{
}

Circle::~Circle() {}

bool Circle::within(float x, float y) const { return sqr(m_x - x) + sqr(m_y - y) <= sqr(r); }

void Circle::draw(QPainter & painter) const { painter.drawEllipse(m_x, m_y, r, r); }
}

