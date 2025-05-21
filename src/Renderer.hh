#pragma once

#include <glad/glad.h>
#include <signal.h>

// Macro for error handling
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define glCall(func) glClearError();\
    func;\
    ASSERT(glLogError(#func, __FILE__, __LINE__))

void glClearError();
bool glLogError(const char* function, const char* file, int line);
