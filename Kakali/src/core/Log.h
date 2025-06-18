#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define DEBUG_CALLBACK false

#ifndef DISTRIBUTION
#define LOG(x) std::cout << x << std::endl
#define ERROR_LOG(x) LOG(x);
#define ASSERT(x, message) if ((x)) LOG(message << " Success! " << __FILE__ << "; " << __LINE__ << "; " << #x); \
								else LOG(message << " Failed! " << __FILE__ << "; " << __LINE__ << "; " << #x);
#define ASSERT_ERROR(x) if (!(x)) LOG("Failed! " << __FILE__ << "; " << __LINE__ << "; " << #x);
#if DEBUG_CALLBACK
    inline void GLErrorCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar * message,
        const void* userParam) {

    }
    #define SET_DEBUG_CALLBACK glDebugMessageCallback(GLErrorCallback, nullptr)
    #define GLCall(x) x;
#else
    #define SET_DEBUG_CALLBACK
    #define GLCall(x) x; ASSERT_ERROR(GLLogCall(#x, __FILE__, __LINE__));
#endif 


inline void GLClearError() {
    while (GLenum error = glGetError());
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "{OpenGL Error} (" << error << "): " << function << " " << file << std::endl;
        return false;
    }
    return true;
}
#else
#define LOG(x)
#define ERROR_LOG(x)
#define ASSERT(x, message)
#define GLCall(x) x;
#if DEBUG_CALLBACK
    inline void GLErrorCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam) {

    }
    #define SET_DEBUG_CALLBACK glDebugMessageCallback(GLErrorCallback, nullptr)
#else
    #define SET_DEBUG_CALLBACK
#endif 
#endif