#pragma once


#include <vector>
#include <list>
#include <map>
#include <deque>
#include <cstdio>
#include <string>
#include <functional>
#include <typeinfo>
#include <memory>

#include <sys/types.h>
#include <sys/stat.h>



#if defined(__ANDROID__)
#include <GLES/gl.h>
#include <GLES/glext.h>


#include "JniFunc.h"

#define glOrtho glOrthof

#elif defined(__IPHONE__)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(WIN32) || defined(__MINGW__)
#include <Windows.h>
#include <glew/glew.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

extern GLFWwindow* window;
#endif

#if defined(WIN32) && !defined(__MINGW__)
#pragma warning (disable:4244)	/* Disable bogus conversion warnings. */
#pragma warning (disable:4100)
#pragma warning (disable:4127)
#pragma warning (disable:4505)
#endif


#include "Configuration.h"
#include "PlatformMacros.h"
#include "Point.h"
#include "Size.h"
#include "Rect.h"
#include "Stack.h"
#include "Queue.h"
#include "Util.h"
#include "Message.h"
#include "Object.h"
#include "Thread.h"
#include "SevenZDec.h"


#include "Node.h"
#include "Touchable.h"
#include "Image.h"
#include "Drawable.h"
#include "Scene.h"
#include "Layer.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Text.h"
#include "RunTime.h"
#include "Scheduler.h"
#include "Director.h"
#include "Application.h"
#include "AppMain.h"



