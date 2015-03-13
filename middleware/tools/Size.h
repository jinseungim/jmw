#pragma once


NS_JMW_BEGIN

class Size
{
public:
	Size(void) : width(0.0), height(0.0) {};
	Size(f64 w, f64 h) : width(w), height(h) {};
	virtual ~Size(void){};

	f64  width;
	f64  height;
};

NS_JMW_END