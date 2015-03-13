#include "jmw.h"
#include "Thread.h"

NS_JMW_BEGIN

Thread::Thread() : mVoid(nullptr), m_hThread(0)
{
}


Thread::~Thread()
{
#if defined(WIN32)
	CloseHandle(m_hThread);
#else

#endif
}

void Thread::Start(void* mParam)
{
	mVoid = mParam;

#if defined(WIN32)
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, &Thread::ThreadRun, (void*)this, 0, (unsigned*)&m_dwThreadID);
#else
	pthread_create(&m_hThread, nullptr, &Thread::ThreadRun, (void*)this);
#endif
}


#if defined(WIN32)
u32 __stdcall Thread::ThreadRun(void* v)
#else
void* Thread::ThreadRun(void* v)
#endif
{
#if defined(WIN32)
	u32 ret0;
#else
	void* ret0;
#endif
	Thread* thread = (Thread*)v;

	thread->Enter(thread->GetParam());

#if defined(WIN32)
	ret0 =  (u32)thread->Run(thread->GetParam());
#else
	ret0 = thread->Run(thread->GetParam());
#endif

	thread->Exit(thread->GetParam());

	return ret0;

}


NS_JMW_END
