#include "Renderer.h"
#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>



Shader::Shader(const std::string& filepath) : m_RendererID(0)
{
    parsedShaders shaders;
    shaders = parseShader(filepath);

    std::string vs = shaders.VertexCode;
    std::string fs = shaders.FragmentCode;

    m_RendererID = CreateShader(vs, fs);
}

/// <summary>
/// //Reading Shader Code from External File
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
parsedShaders Shader::parseShader(const std::string& path)
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

    return { shaderCodeStream[0].str(), shaderCodeStream[1].str() };
}

/// <summary>
/// //Method to compile shader source code(for both Vertex and Fragment Shaders)
/// </summary>
/// <param name="type"></param>
/// <param name="source"></param>
/// <returns></returns>
unsigned int Shader :: CompileShader(unsigned int type, const std::string& source)
{

    std::cout << source << std::endl;

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;

        // Function get lenght of log in bytes
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        //alloc allocate continous memory in fashion of stack and not heap
        char* message = (char*)alloca((sizeof(char)) * length);

        //Get Shader error Log
        glGetShaderInfoLog(id, length, &length, message);


        std::cout << "Error : " << ((type == GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader") << " compliation failed " << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/// <summary>
/// Method for combining, linking and validating Compiled shaders code to single program obj
/// </summary>
/// <param name="VertexShader"></param>
/// <param name="FragmentShader"></param>
/// <returns></returns>
unsigned int Shader :: CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
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

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUnifom4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUnifom4i(const std::string& name, int v0, int v1, int v2, int v3)
{
    GLCall(glUniform4i(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation( const std::string& name)const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

    if (location == -1)
    {
        std::cout << "uniform " + name + " dosen't exists";
    }

    m_UniformLocationCache[name] = location;
    return location;
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}