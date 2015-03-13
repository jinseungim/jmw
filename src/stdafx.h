// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
//#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <CommCtrl.h>
#include <crtdbg.h>
#include <stdio.h>
//#include <strsafe.h>
#include <MMSystem.h>
#include <WinSock2.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "time.h"
#include "math.h"
#include "lib\fmod\include\fmod.hpp"
//#include "fmod.h"

#include "AppInfo.h"

#pragma comment(lib, "ws2_32.lib")										//winsock
//#pragma comment(lib, "Advapi32.lib")									//timeGetTime
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, ".\\lib\\fmod\\fmodex_vc.lib")
#pragma comment(lib, ".\\lib\\freetype\\freetype244MT_D.lib")


#pragma warning (disable:4244)	/* Disable bogus conversion warnings. */
#pragma warning (disable:4305)  /* VC++ 5.0 version of above warning. */

#pragma warning (disable:4100)  // �������� �ʴ� �Ű�����
#pragma warning (disable:4127)  // ���ǽ��� ���



extern HWND g_hWnd;

extern char CONFIGURE_TITLE[0xff];
extern char CONFIGURE_UUID[0xff];
extern char CONFIGURE_IP[40];
extern int CONFIGURE_PORT;
extern int CONFIGURE_LANG;
extern int CONFIGURE_APP_IDX;

extern int DEVICE_WIDTH;
extern int DEVICE_HEIGHT;

extern int CONFIGURE_USE_CONSOLE;
extern int CONFIGURE_USE_APP_INFO;

using namespace FMOD;


/*
#ifdef _DEBUG
    #define MYDEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
    //allocations to be of _CLIENT_BLOCK type
#else
    #define MYDEBUG_NEW
#endif // _DEBUG


#ifdef _DEBUG
	#define new MYDEBUG_NEW

	#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

	*/