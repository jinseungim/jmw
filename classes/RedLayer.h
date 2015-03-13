#pragma once

#include "jmw.h"


USING_NS_JMW;


class RedLayer : public Layer
{
public:
	RedLayer();
	~RedLayer();

	void OnDraw(sl32);
};

