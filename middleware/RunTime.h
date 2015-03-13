#pragma once

#if defined(__ANDROID__)
#include <sys/time.h>
#elif defined(WIN32) || defined(__MINGW__)
#include <Windows.h>
#endif


NS_JMW_BEGIN

#define MILLISEC 1000

class RunTime
{
    private:
        RunTime(void);
        ~RunTime(void);

    static RunTime *pInstance;

    private:
#if defined(__ANDROID__)
    struct timespec m_lasttime;
#elif defined(WIN32) || defined(__MINGW__)
	DWORD m_lasttime;
#endif

    public:
        static RunTime& GetInstance(void);
        static void ReleaseInstance(void);

        void Init(void);
        sl32 GetDelta(void);
		s64  GetCurTime(void);
		pcsz GetStringTime(s64 time);
};

NS_JMW_END
