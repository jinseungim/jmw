#include <windows.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;



void handleResize(GLFWwindow* window, int width,int height)
{

    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION );

    glLoadIdentity();

    glOrtho( 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f );
    //glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}




static void Render() {
    glClearColor(0, 0, 0, 1.0); // black
    glClear(GL_COLOR_BUFFER_BIT);


       GLfloat points[] = {
        0.2, 0.2, 0.0,  //: 좌-하단
        0.8, 0.2, 0.0,  //: 우-하단
        0.2, 0.8, 0.0,  //: 좌-상단
        0.8, 0.8, 0.0   //: 우-상단
    };

    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective



    glVertexPointer(3, GL_FLOAT, 0, points);
	glEnableClientState(GL_VERTEX_ARRAY);
	{
        //: 점의 크기를 설정한다
		glPointSize(10);
        //: 점의 색상을 설정한다
		glColor4f(1.0, 1.0, 1.0, 1.0);
        //: 정점 배열의 내용을 점으로 그린다
		glDrawArrays(GL_POINTS, 0, 4);
	}
	glDisableClientState(GL_VERTEX_ARRAY);

}


void AppMain() {

    GLFWwindow* window;
    int width, height;
    // initialise GLFW
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    glEnable(GL_DEPTH_TEST);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }


    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, handleResize); //callback function of GLFW to handle window resize



    glfwGetWindowSize(window, &width, &height);
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f );



    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        Render();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();


}



int main()
{
    try {

        AppMain();

    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
