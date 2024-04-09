#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
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
    window = glfwCreateWindow(960, 540, "Graphics Engine", NULL, NULL);
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

    ///ImGUI Setup
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsDark();


{
    
    //drawing square using indices without drawing duplicate vertices
    float positions[] =
    {
                        //Sampling Texture Coordinates(Mapping Coordinates)
                       //(Pixels to retrieve from texture, for each vertex)
         240.0f,  30.0f, 0.0f,0.0f,       //0    
         240.0f,510.0f, 0.0f,1.0f,       //1
         720.0f,510.0f, 1.0f,1.0f,       //2
         720.0f,  30.0f, 1.0f,0.0f        //3
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

    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(1.0f);

  //  glm::vec4 vp = glm::vec4(100.0f, 100.0f, 0.0f, 1.0f);
  // glm::vec4 res = proj * vp;

    float r = 0.1f, g = 0.5f, b = 1.0f;
    
    glm::vec3 color = glm::vec3(r, g, b);

    //u_ Naming Convention for Uniforms
    shader.SetUnifom4f("u_Color", color.r, color.g, color.b, 1.0f);
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

        //Imgui New frame
        ImGui_ImplGlfwGL3_NewFrame();

        if (color.r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (color.r < 0.0f)
        {
            increment = 0.05f;
        }

        color.r += increment;


        ///mvp calculations

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //For camera
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        //Scaling tried but pivot is not correct
        //scaling, translating & rotating
        glm::mat4 model = glm::scale(glm::mat4(1.0f), scale);
        model = glm::translate(model, translate);
        //model = glm::rotate(model, 90.0f, rotation);

        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        shader.SetUnifom4f("u_Color", color.r, color.g, color.b, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(ib, va, shader);


        //Imgui New Window Settings
        {
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Text("Kimi no na wa");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat2("2D Scale", &scale.x, 0.1f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::SliderFloat2("2D Translate", &translate.x, -480.0f, 480.0f);
            //ImGui::SliderFloat3("Color", &color.x, 0.0f, 255.0f);
            ImGui::ColorEdit3("clear color", (float*)&color); // Edit 3 floats representing a color
            //ImGui::SliderFloat2("2D Rotate", &rotation.x, 0.1f, 500.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
        
        //Draw Command To Draw imgui Window
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

    //Shutdown Command for Imgui
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
};