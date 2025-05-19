#ifndef SHADER_HH
#define SHADER_HH

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>


namespace Shader {
    struct ShaderSource {
        std::string vertexSource;
        std::string fragmentSource;
    };

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    ShaderSource parseShader(const std::string& filePath);    
    unsigned int createShader(const std::string& vertexSource,
                              const std::string& fragmanetSource);
    unsigned int compileShader(unsigned int type,const std::string& source);
}


#endif // SHADER_HH
