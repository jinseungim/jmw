#pragma once


NS_JMW_BEGIN

class Point
{
public:
	Point(void): xpos(0.0), ypos(0.0) {};
	Point(f64 x, f64 y) : xpos(x), ypos(y) {};
	virtual ~Point(void){};

	f64  xpos;
	f64  ypos;
};

NS_JMW_END