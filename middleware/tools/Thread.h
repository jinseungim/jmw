  #pragma once

#if defined(WIN32)
#include <process.h>
#else
#include <pthread.h>
#endif



NS_JMW_BEGIN



class Thread : public Object
{
public:
	Thread();
	virtual ~Thread();

	void Start(void*);

	virtual void Enter(void*) {};
	virtual void* Run(void*) { return 0; };
	virtual void Exit(void*) {};


	void* GetParam() const { return mVoid; }



private:
	void* mVoid;

#if defined(WIN32)
	static u32 __stdcall ThreadRun(void*);
	HANDLE m_hThread;
	DWORD  m_dwThreadID;
#else
	static void* ThreadRun(void*);
	pthread_t m_hThread;
#endif
};

NS_JMW_END
