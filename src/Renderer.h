#pragma once

#include <GL/glew.h>
#include<iostream>

// Macro to apply breakpoint at runtime if soln is on Debug
#define ASSERT(x) if(!(x)) __debugbreak();

//Macro to wrap GlClear, Glogcall and Assert forparticular func x
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))



/// <summary>
/// Method to map error code to repective messages
/// </summary>
/// <param name="error"></param>
/// <returns></returns>
std::string getGLErrorString(GLenum error);

/// <summary>
/// Method To Clear All Previous Error Logs
/// </summary>
void GLClearError();

/// <summary>
/// Method to collect get log errors
/// </summary>
/// <param name="functionName"></param>
/// <param name="filePath"></param>
/// <param name="line"></param>
/// <returns></returns>
bool GLLogCall(const char* functionName, const char* filePath, long line);

