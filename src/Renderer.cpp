#include "Renderer.h"
#include <iostream>


std::string getGLErrorString(GLenum error)
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

void GLClearError()
{
    //glgetError basically return current error codes and set error flag to GL_NO_ERROR
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* functionName, const char* filePath, long line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Open GL]" << "v" << glGetString(GL_VERSION) << std::endl
            << "[File] : " << filePath << std::endl
            << "[ERROR] :- " << getGLErrorString(error) <<
            "\n[FUNCTION] " << functionName << " on line " << line << std::endl;
        return false;
    }

    return true;
}