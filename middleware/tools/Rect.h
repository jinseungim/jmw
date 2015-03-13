#pragma once

NS_JMW_BEGIN

class Rect
{
public:
	Rect(void) : m_Pos(0, 0), m_Size(0, 0) {};
	Rect(Point p, Size s) : m_Pos(p), m_Size(s) {};
	virtual ~Rect(void){};

	Point  m_Pos;
	Size   m_Size;

	bl Collision(const Point& point);
};

NS_JMW_END

