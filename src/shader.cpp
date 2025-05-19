#include "shader.hh"

//------------------------------------------------------------------------------
// Public

Shader::ShaderSource Shader::parseShader(const std::string& filePath) {
    ShaderType currentType = ShaderType::NONE;
    std::stringstream shaderSource[2];

    std::ifstream fileObj(filePath);
    if (!fileObj) {
        throw std::runtime_error("Error reading from file! -> "
                                 + filePath);
    }

    std::string line;
    while (std::getline(fileObj, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                currentType = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                currentType = ShaderType::FRAGMENT;
        }
        else
        {
            shaderSource[(int)currentType] << line << "\n";
        }
    }

    return ShaderSource{shaderSource[0].str(), shaderSource[1].str()};
}

unsigned int Shader::createShader(const std::string &vertexSource,
                                  const std::string &fragmentSource)
{
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
{
    unsigned int shader = glCreateShader(type);
    const char* shaderSource = source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Shader Compilation failed!!!\n")
                                             + infoLog);
    }

    return shader;
}
