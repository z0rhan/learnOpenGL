#include "IndexBuffer.hh"
#include "Renderer.hh"
#include "Shader.hh"
#include "Texture.hh"
#include "VertexArray.hh"
#include "VertexBuffer.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const std::string c_shaderFilePath = "res/shaders/basic.shader";
const std::string c_textureFilePath1 = "res/textures/container.jpg";
const std::string c_textureFilePath2 = "res/textures/awesomeface.png";

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // OpenGL version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    Renderer renderer;

    // Make shader object
    Shader shader(c_shaderFilePath);
    if (!shader.isValid())
    {
        std::cerr << "Something went wrong while initializing the shader!"
                  << std::endl;
        return EXIT_FAILURE;
    }

    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 1.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
        };

        unsigned int indices[] = {0, 1, 2, 1, 2, 3};

        glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f);

        // Vertex Array Object
        VertexArray VAO;

        // Vertex Buffer Object
        VertexBuffer VBO(positions, sizeof(positions));

        // Element Buffer Object
        IndexBuffer EBO(indices, 6);

        VertexBufferLayout layout;
        layout.push<float>(2); // add the positions layout
        layout.push<float>(2); // add the texture layout
        // layout.push<float>(3) -> say we decide to add colors too
        VAO.addBuffer(VBO, layout); // this does -> VAO.bind() & VBO.bind()

        shader.bind();

        Texture texture1(c_textureFilePath1);
        Texture texture2(c_textureFilePath2);
        texture1.bind();
        texture2.bind(1);
        shader.setUniform1i("u_texture1", 0);
        shader.setUniform1i("u_texture2", 1);
        shader.setUniformMat4f("u_MVP", proj);

        while (!glfwWindowShouldClose(window))
        {
            // Handle Input
            processInput(window);

            // Handle Render
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            renderer.draw(VAO, EBO, shader);

            // Swap Buffer
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
