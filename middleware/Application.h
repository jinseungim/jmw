#pragma once

NS_JMW_BEGIN
class Application
{
public:
	Application(void){};
	virtual ~Application(void){};


	virtual bl AppDidFinishLaunching(void) = 0;
};

NS_JMW_END
