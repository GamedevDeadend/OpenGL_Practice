#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
	std::cout << "Rendering..." << std::endl;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Graphics Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    if (glewInit() == GLEW_OK)
    {
        std::cout << "Init Success" << std::endl;
        std::cout << glGetString(GL_VERSION)  << std::endl;
    }

    unsigned int buffer = 7;

    //for blocking buffer
    glGenBuffers(1, &buffer);

    // for selecting buffer and use it as array
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    float positions[] =
    {
        (-0.5f,-0.5f),
        (0.0f,0.5f),
        (0.5f,-.5f)
    };

    //Specify size and give data
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*Draw Triangle using Legacy OpenGL*/

        //Draw Call to draw triangle using assigned buffer
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Another Way for DrawCall that we will use later on
        //glDrawElements(GL_TRIANGLES, 6, GL_STATIC_DRAW);

        //glBegin(GL_TRIANGLES);
        //glVertex2f( -0.5f,-0.5f);
        //glVertex2f( 0.0f,0.5f);
        //glVertex2f( 0.5f,-.5f);
        //glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}