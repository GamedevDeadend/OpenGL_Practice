#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc( (sizeof(char)) * length);
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Error : " << ( (type == GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader" )<<" compliation failed "<< std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }


    return id;

}

static unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




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

 /*  
     for selecting buffer and use it as array
     (example : 
                  photoshop if i select layer then i can't draw on another layer
                  similary we will give draw call for selected buffer only
     )
*/

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    float positions[] =
    {
        -0.5f,-0.5f,
        0.0f,0.5f,
        0.5f,-.5f
    };

    //Specify size and give data
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    //Vertex(Not necessay pos it can have manyother attributes)->attributes->Components

    //Function to define Attribute of vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    //To Enable Vertex Attribute of particular Index
    glEnableVertexAttribArray(0);

    std::string vs =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fs =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.875,0.388,0.929,1.0);\n"
        "}\n";

    unsigned int program =  CreateShader(vs, fs);
    glUseProgram(program);

    /* Loop until the user closes the window */
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        //Draw Call to draw triangle using assigned buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //Another Way for DrawCall that we will use later on
        //glDrawElements(GL_TRIANGLES, 6, GL_STATIC_DRAW);


        /*Draw Triangle using Legacy OpenGL*/
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