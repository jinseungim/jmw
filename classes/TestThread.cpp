#include "jmw.h"
#include "TestThread.h"


TestThread::TestThread()
{
}


TestThread::~TestThread()
{
}


void TestThread::Enter(void* param)
{
	LOG("%s", "Thread Enter");
}


void*  TestThread::Run(void* param)
{
	LOG("%s", "Thread Run");

	return 0;
}


void TestThread::Exit(void* param)
{
	LOG("%s", "Thread Exit");
}