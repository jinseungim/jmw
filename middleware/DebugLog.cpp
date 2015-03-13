#include "Configuration.h"
#include "DebugLog.h"
#include <iostream>
#include <string>



void jni_android_assert(bl x, const sz* file, s32 line) {

#if defined(__ANDROID__)
	if (x == false)
	{
		__android_log_print(ANDROID_LOG_ERROR, "_log_", "assert : %s (%d)\n", file, line);
	}
#endif
}


#if defined(WIN32) || defined(__MINGW__)
static GameErrLog gel;


void WriteLog(const sz* file, const sz* func, s32 line, const sz* fmt, ...)
{

	sz buffer[1024] = { 0, };
	std::va_list va;
	va_start(va, fmt);
	vsprintf_s(buffer, 1024, fmt, va);
	va_end(va);

	std::cout << buffer << std::endl;



	if (WRITE_TOFILE_LOG != 0)
		gel.Write(file, func, line, buffer);

}



GameErrLog::GameErrLog() : _year(0), _mon(0), _day(0) {
	Init();
}

GameErrLog::~GameErrLog() {
}

void GameErrLog::Init(void) {
}

void GameErrLog::ChangeFile() {

	time_t current;
    time(&current);
    tm *today = localtime(&current);


	if (today->tm_year + 1900 != _year || today->tm_mon + 1 != _mon || today->tm_mday != _day) {

    	_year = today->tm_year + 1900;
		_mon  = today->tm_mon + 1;
		_day  = today->tm_mday;

		TCHAR path[0xff];
		TCHAR name[0xff];

		GetModuleFileName(NULL, path, sizeof(path));

		
		TCHAR* pos = wcsrchr(path, L'\\');
		path[(pos - path)+1] = '\0';
		

		swprintf(name, L"%d-%02d-%02d.txt", _year, _mon, _day);
		wcscat(path, name);
		wcscpy_s(_filename, path);
	}
}



void GameErrLog::Write(const sz* file, const sz* func, s32 line, const sz *Msg) {

	sz log[4096];
	FILE *pFile;
	char ctemp[256] = { 0, };


	
	ChangeFile();

	//WideCharToMultiByte(CP_ACP, 0, _filename, wcslen(_filename), ctemp, sizeof(ctemp), NULL, NULL);
	pFile = _wfopen(_filename, L"a+");


	if (pFile) {

		time_t current;
		time(&current);
		tm *today = localtime(&current);


		sprintf(log, "[%d-%d-%d %02d:%02d:%02d] %s\n[function] %s (%d line)\n[log] %s\n\n",
			today->tm_year + 1900, today->tm_mon + 1, today->tm_mday, today->tm_hour, today->tm_min, today->tm_sec,  file,
			func, line,
			Msg);

		s32 len = strlen(log);
		fwrite(log, len, 1, pFile);
		fclose(pFile);
	}
}
#endif
