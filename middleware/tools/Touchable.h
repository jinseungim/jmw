#pragma once


NS_JMW_BEGIN

class Touchable
{
	
public:
	Touchable(void) {};
	virtual ~Touchable(void) {};

	virtual bl OnTouchDown(Point) = 0;
	virtual bl OnTouchMove(Point) = 0;
	virtual bl OnTouchUp(Point)   = 0;

};





NS_JMW_END