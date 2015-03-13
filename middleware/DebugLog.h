#pragma once


#include <cstdio>
#include <ctime>
#include <assert.h>

#if defined(__ANDROID__)
#include <android/log.h>
#else
#include <cstdarg>
#include <Windows.h>
#endif


#ifdef __MINGW__
#define vsprintf_s vsnprintf
#endif



void jni_android_assert(bl x, const sz* file, s32 line);
void WriteLog(const sz* file, const sz* func, s32 line, const sz* fmt, ...);



#if DEBUG_LEVEL >= 10
    #if defined(WIN32) || defined(__MINGW__)
        #define ASSERT(x)  assert(x)
    #elif defined(__ANDROID__)
        #define ASSERT(x)  jni_android_assert(x, __FILE__, __LINE__)
    #endif
#else
        #define ASSERT(x) /* NOP */
#endif

// 디버그정보 형식을 프로그램 데이터베이스(/Zi)으로 설정해야지만 __LINE__ 매크로를 정상적으로 사용할수 있다.
#if DEBUG_LEVEL >= 30
    #if defined(WIN32) && !defined(__MINGW__)
        #define LOG(fmt, ...) WriteLog(##__FILE__, ##__FUNCTION__, ##__LINE__, fmt, ##__VA_ARGS__)
    #elif defined(__MINGW__)
        #define LOG(fmt, ...) WriteLog(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
    #elif defined(IPHONE)
        #define	LOG(...) fprintf(stderr, __VA_ARGS__)
    #elif defined(__ANDROID__)
        #define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG,"_log_",__VA_ARGS__)
    #endif
#else
        #define LOG(...) /* NOP */
#endif





#if defined(WIN32) || defined(__MINGW__)
class GameErrLog
{
private:


	TCHAR   _filename[0xff];

	s32 _year;
	s32 _mon;
	s32 _day;
public:
	GameErrLog(void);
	~GameErrLog(void);

	void Init(void);

	void ChangeFile(void);
	void Write(const sz* , const sz* , s32 , const sz*);

};
#endif
