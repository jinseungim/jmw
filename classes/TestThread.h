#pragma once

USING_NS_JMW;

class TestThread : public Thread
{
public:
	TestThread();
	~TestThread();


	void Enter(void*);
	void*  Run(void*);
	void Exit(void*);


};

