#pragma once

#include <glad/glad.h>

#ifdef _WIN32
    #include <windows.h>
    #define ASSERT(x) if (!(x)) __debugbreak();
    #define glCall(func)\
        glClearError();\
        func;\
        ASSERT(glLogError(#func, __FILE__, __LINE__))
#else
    #include <csignal>
    #define ASSERT(x) if (!(x)) raise(SIGTRAP);
    #define glCall(func)\
        glClearError();\
        func;\
        ASSERT(glLogError(#func, __FILE__, __LINE__))
#endif

void glClearError();
bool glLogError(const char *function, const char *file, int line);

