#include "glm/trigonometric.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <format>
#include <filesystem>

void updateWindowSize(GLFWwindow* window, int width, int height);

const char* c_vertexShaderSource =
"#version 460 core\n"
"\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texture;\n"
"\n"
"out vec2 v_texCoord;\n"
"\n"
"uniform mat4 u_transform;\n"
"void main()\n"
"{\n"
"   gl_Position = u_transform * vec4(position, 1.0);\n"
"   v_texCoord = texture;\n"
"}\n";

const char* c_fragmentShaderSource =
"#version 460 core\n"
"\n"
"out vec4 FragColor;\n"
"in vec2 v_texCoord;\n"
"\n"
"uniform sampler2D u_texture1;\n"
"uniform sampler2D u_texture2;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = mix(texture(u_texture1, v_texCoord), texture(u_texture2, v_texCoord), 0.3);\n"
"}\n";

const char* c_texture1FilePath = "res/textures/container.jpg";
const char* c_texture2FilePath = "res/textures/awesomeface.png";

int main (int argc, char *argv[]) {
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "TEST", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to initialize window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!\n";
        return EXIT_FAILURE;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, updateWindowSize);

    std::cout << std::format("Wokring dir: {}\n", std::filesystem::current_path().string());

    float attributes[] = {
        -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, // Top left
         0.5f,  0.5f,  0.0f, 1.0f,  1.0f, // Top right
        -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, // Bottom left
         0.5f, -0.5f,  0.0f, 1.0f,  0.0f  // Bottom right 
    };

    unsigned int indices[] = {
        0, 1, 3,
        0, 2, 3
    };

    // Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(attributes), attributes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Element Buffer Object
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &c_vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << std::format("Shader Compilation failes: {}", infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &c_fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << std::format("Shader Compilation failes: {}", infoLog);
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Texture
    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width1, height1, BPP1;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(c_texture1FilePath, &width1, &height1, &BPP1, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cerr << "Failed to load texture\n";
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    data = stbi_load(c_texture2FilePath, &width1, &height1, &BPP1, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cerr << "Failed to load texture\n";
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
    model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture2"), 1);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_transform"), 1, GL_FALSE, glm::value_ptr(model));

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << "\n";
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_transform"), 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}


void updateWindowSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
