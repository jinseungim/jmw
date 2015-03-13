#include <windows.h>
#include <cassert>
#include <iostream>
#include <stdexcept>

#if defined(WIN32) && !defined(__MINGW__)

#pragma warning (disable:4244)	/* Disable bogus conversion warnings. */
#pragma warning (disable:4100)

#include <crtdbg.h>
#endif


#include "jmw.h"
#include "AppDelegate.h"

#include "TestCase.h"


using namespace std;
USING_NS_JMW;



AppDelegate app;

int width, height;
double g_xpos, g_ypos;

static void error_callback(int error, const char* description)
{
    LOG("err: %d des: %s", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	AppMain::GetInstance().Resize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	//LOG("button %d action %d mods %d", button, action, mods);
	AppMain::GetInstance().HandleTouch(action, 0, g_xpos, g_ypos);
}


void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	//LOG("xpos %f ypos %f", xpos, ypos);
	g_xpos = xpos, g_ypos = height - ypos;

	AppMain::GetInstance().HandleTouch(TOUCH_MOVE, 0, g_xpos, g_ypos);
}

void focus_callback(GLFWwindow *window, int focused)
{
	LOG("focused %d", focused);
}

void refresh_callback(GLFWwindow *window)
{
	//LOG("refresh_callback");
}

GLFWwindow* window;
void RunMain() {

    
    

    glfwSetErrorCallback(error_callback);


    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    //glEnable(GL_DEPTH_TEST);

	window = glfwCreateWindow(639, 360, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }



	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetWindowFocusCallback(window, focus_callback);
	//glfwSetWindowRefreshCallback(window, refresh_callback);

	glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &width, &height);

	GetDirector().SetScreenSize(width, height);
	GetDirector().SetGameWidth(width);
	GetDirector().SetGameHeight(height);
	if (AppMain::GetInstance().Init(&app))
	{
		AppMain::GetInstance().Resize(width, height);

		while (!glfwWindowShouldClose(window))
		{
			AppMain::GetInstance().Run();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

    glfwTerminate();
	AppMain::GetInstance().ReleaseInstance();
}


SevenZDec sz7("d:/test/test.7z", "d:/7ztest_mingw");
int main(int argc, char** argv)
{
    int exit = EXIT_SUCCESS;
#if defined(WIN32) && !defined(__MINGW__)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc( 938 );
#endif

    try {

		if (GMOCK_ENABLE == 1)
		{
			testing::InitGoogleTest(&argc, argv);
			exit = RUN_ALL_TESTS();
		}



		sz7.Start(nullptr);

        RunMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit = EXIT_FAILURE;
    }


    return exit;
}

