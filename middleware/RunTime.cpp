#include "jmw.h"
#include "RunTime.h"


NS_JMW_BEGIN

RunTime *RunTime::pInstance = nullptr;

RunTime& RunTime::GetInstance(void)
{
	if (pInstance == nullptr)
		pInstance = new RunTime();

	return *pInstance;
}


void RunTime::ReleaseInstance(void)
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


RunTime::RunTime(void)
{

#if defined(__ANDROID__)
	clock_gettime(CLOCK_REALTIME, &m_lasttime);
#elif defined(WIN32) || defined(__MINGW__)
	m_lasttime = 0;
#endif
}

RunTime::~RunTime(void)
{
}


void RunTime::Init(void)
{
}


sl32 RunTime::GetDelta(void)
{
    sl32 delta=0;

#if defined(__ANDROID__)
    struct timespec curtime;

    clock_gettime(CLOCK_REALTIME, &curtime);
    delta = ((curtime.tv_sec - m_lasttime.tv_sec)*MILLISEC + (curtime.tv_nsec - m_lasttime.tv_nsec)/CLOCKS_PER_SEC)+0.5;


#elif defined(WIN32) || defined(__MINGW__)
	DWORD curtime;
	curtime = timeGetTime();

	delta = (m_lasttime > 0) ? (curtime - m_lasttime) : 0;
#endif

    m_lasttime = curtime;

    return delta;

}


s64 RunTime::GetCurTime(void)
{
    s64 curtime;
#if defined(__ANDROID__)
    struct timespec tspec;
    clock_gettime(CLOCK_REALTIME, &tspec);

    curtime = tspec.tv_sec;

#elif defined(WIN32) || defined(__MINGW__)
        time(&curtime);
#endif


    return curtime;
}


pcsz RunTime::GetStringTime(s64 time)
{
	static char buf[50];
	size_t sz_buf = sizeof(buf);
	memset(buf, 0, sz_buf);

#if defined(__ANDROID__)
	struct tm   tm_now;

	localtime_r((time_t *)&time, &tm_now);
	LOG("%d", tm_now.tm_hour);
	strftime(buf, sz_buf, "%y-%m-%d %H:%M:%S", &tm_now);
#elif defined(WIN32) || defined(__MINGW__)
	strftime(buf, sz_buf, "%Y-%m-%d %H:%M:%S", localtime(&time));
#endif

	return buf;
}

NS_JMW_END
