#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

int main(void)
{
    std::cout << "Rendering..." << std::endl;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Graphics Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //Initializing Glew for OpenGL glew is library (GLEW is common library of abstraction of OpenGL implementation for all platforms) 
    glewInit();

    if (glewInit() == GLEW_OK)
    {
        std::cout << "Init Success" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

{
    
    //drawing square using indices without drawing duplicate vertices
    float positions[] =
    {
                        //Sampling Texture Coordinates(Mapping Coordinates)
                       //(Pixels to retrieve from texture, for each vertex)
         -0.5f,-0.5f,  0.0f,0.0f,       //0    
         -0.5f, 0.5f,  0.0f,1.0f,       //1
          0.5f, 0.5f,  1.0f,1.0f,       //2
          0.5f,-0.5f,  1.0f,0.0f        //3
    };

    //indices Method
    unsigned int indices[] =
    {
        0,1,2,
        2,3,0
    };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));


    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexArray va;

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);



    //INDICES BUFFER
    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    float r = 0.1f, g = 0.5f, b = 1.0f;
    //u_ Naming Convention for Uniforms
    shader.SetUnifom4f("u_Color", r, g, b, 1.0f);
    shader.SetUniform1i("u_Texture", 0);

    Texture texture("res/textures/pngegg.png");
    texture.Bind();



    //Unbinding all objects
    va.UnBind();
    shader.UnBind();
    ib.UnBind();
    vb.UnBind();


    float increment = 0.05f;

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        if (r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        shader.Bind();
        shader.SetUnifom4f("u_Color", r, g, b, 1.0f);
        renderer.Draw(ib, va, shader);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

    glfwTerminate();
    return 0;
};