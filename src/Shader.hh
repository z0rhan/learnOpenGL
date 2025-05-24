#ifndef SHADER_HH
#define SHADER_HH

/*
 * This is more of a note for myself
 *
 * Shader class to deal with shaders [Abstractions]
 * - glCreateShader(shadertype[GL_VERTEX_SHADER,...]) -> uint
 * - glShaderSource(unit, &source, nullptr)
 * - glCompileShader(unit)
 * - glDeleteShader(shader)
 *
 * - glCreateProgram() -> uint
 * - glAttachShader(shaderProgram, shader)
 * - glLinkProgram(shaderProgram)
 * The constructor does all the above and stores the shaderProgram as m_renderedId
 *
 * bind() calls glUseProgram(shaderProgram)
 * unbind() calls glUseProgram(0)
 *
 * - glGetUniformLocation(shaderProgram, name)
 * - glUniform4f(uniformLocation, vec4f)
 * setUniform4f() does the above calls to deal with uniforms
 *
 * - glDeleteProgram(shaderProgram)
 * The destructor does the above call
 */


#include <string>
#include <unordered_map>

struct ShaderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

enum class ShaderType
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
};

class Shader {
public:
    Shader(const std::string& filePath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string& name, int value);

    bool isValid() const;

private:
    std::string m_filePath; // For debugging purposes
    unsigned int m_renderedId;
    std::unordered_map<std::string, int> m_uniformLocationCache;

    static ShaderSource parseShader(const std::string& filePath);    
    static unsigned int createShader(const std::string& vertexSource,
                                     const std::string& fragmentSource);
    static unsigned int compileShader(unsigned int type,const std::string& source);
    int getUniformLocation(const std::string& name);
};


#endif // SHADER_HH
