#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<fstream>
#include<string>
#include<sstream>


// Macro to apply breakpoint at runtime if soln is on Debug
#define ASSERT(x) if(!(x)) __debugbreak();

//Macro to wrap GlClear, Glogcall and Assert forparticular func x
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// function to map error code to repective messages
static std::string getGLErrorString(GLenum error) 
{
    switch (error) 
    {
    case GL_NO_ERROR:
        return "No error";

    case GL_INVALID_ENUM:
        return "Invalid enum";

    case GL_INVALID_VALUE:
        return "Invalid value";

    case GL_INVALID_OPERATION:
        return "Invalid operation";

    case GL_STACK_OVERFLOW:
        return "Stack overflow";

    case GL_STACK_UNDERFLOW:
        return "Stack underflow";

    case GL_OUT_OF_MEMORY:
        return "Out of memory";

    case GL_TABLE_TOO_LARGE:
        return "Table too large";

    default:
        return "Unknown error";
    }
}

// Method to clear error log
static void GLClearError()
{
    //glgetError basically return current error codes and set error flag to GL_NO_ERROR
    while (glGetError() != GL_NO_ERROR);
}

/// <summary>
/// Method to collect get log errors
/// </summary>
/// <param name="functionName"></param>
/// <param name="filePath"></param>
/// <param name="line"></param>
/// <returns></returns>
static bool GLLogCall(const char* functionName, const char* filePath, long line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Open GL]" << "v" << glGetString(GL_VERSION)<<std::endl
                  <<"[File] : "<<filePath <<std::endl
                  <<"[ERROR] :- " << getGLErrorString(error) <<
                    "\n[FUNCTION] "<< functionName<<" on line "<<line<<std::endl;
        return false;
    }

    return true;
}


struct parsedShaders 
{
    std::string VertexCode;
    std::string FragmentCode;
};


static parsedShaders parseShader(const std::string&  path)
{
    std::ifstream stream(path);

    enum ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };


    std::string line;
    std::stringstream shaderCodeStream[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("shader") != std::string::npos)
        {

            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            shaderCodeStream[(int)(type)] << line << '\n';
        }
    }

   return { shaderCodeStream[0].str(), shaderCodeStream[1].str()};
}

//Method to compile shader source code
static unsigned int CompileShader(unsigned int type, const std::string& source)
{

    std::cout << source << std::endl;

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        int length;

        // Function get lenght of log in bytes
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        //alloc allocate continous memory in fashion of stack and not heap
        char* message = (char*)alloca( (sizeof(char)) * length);

        //Get Shader error Log
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
    //Create program Object t which shader can be attached and deattached easily
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

    //Attaching vertex shader to program object
    glAttachShader(program, vs);
    //Attaching fragment shader to program object
    glAttachShader(program, fs);

    //Linking for program object
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
        std::cout << glGetString(GL_VERSION)  << std::endl;
    }


    //drawing square using indices without drawing duplicate vertices
    float positions[] =
    {
         -0.5f,-0.5f,//0
         -0.5f, 0.5f,//1
          0.5f, 0.5f,//2
          0.5f,-0.5f //3
    };

    //indices Method
    unsigned int indices[] =
    {
        0,1,2,
        2,3,0
    };

    //Drawing 2 triangles (Square) using old Vertices method without indices
    /*float positions[] =
    {
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.5f,0.5f,

         0.5f, 0.5f,
         -0.5f,0.5f,
         -0.5f,-0.5f
    };*/


   unsigned int buffer;
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

    //Specify size and give data
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    //Vertex(Not necessay pos it can have manyother attributes)->attributes->Components

    //Vertex Array
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    //To Enable Vertex Attribute of particular vertex Index
    GLCall(glEnableVertexAttribArray(0));

    //Function to define Attribute of vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);


    //INDICES BUFFER

        unsigned int ibo;
    //for blocking Index buffer
    glGenBuffers(1, &ibo); //ibo  = Indices Bounding Objects

     /*
     for selecting buffer and use it as array
     (example :
                  photoshop if i select layer then i can't draw on another layer
                  similary we will give draw call for selected buffer only
     )
*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    //Specify size and give data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //Vertex(Not necessay pos it can have manyother attributes)->attributes->Components

    //Function to define Attribute of vertex
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    //To Enable Vertex Attribute of particular vertex Index
    //glEnableVertexAttribArray(0);


    parsedShaders shaders;
    shaders = parseShader("res/shaders/Basic.shader");

    std::string vs = shaders.VertexCode;
    std::string fs = shaders.FragmentCode;

    std::cout << vs << std::endl << fs << std::endl;

    unsigned int program =  CreateShader(vs, fs);

    //To Use Program Object
    glUseProgram(program);


    ///Uniforms : uniforms are way to send data to gpu shader code from cpu
    GLCall(int location = glGetUniformLocation(program, "u_Color");)
        ASSERT(location != -1)

    //Unbinding all objects
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


    float r = 0.1f, g = 0.5f, b = 1.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        //Draw Call to draw triangle using assigned buffer
       // glDrawArrays(GL_TRIANGLES, 0, 6);

        if( r > 1.0f)
        {
           increment = -0.05f;
        }
        else if ( r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        GLCall(glUseProgram(program));
        GLCall(glUniform4f(location, r, g, b, 1.0f));

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        //Indices
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
       

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