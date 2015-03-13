#pragma once


#include "jmw.h"


USING_NS_JMW;


class TestLayer : public Layer
{
public:
	TestLayer();
	~TestLayer();


	void OnEnter();

	bl Sprite0TouchDown(Point);

	void OnDraw(sl32);
	//bl MyCmd(Msg*);

};

