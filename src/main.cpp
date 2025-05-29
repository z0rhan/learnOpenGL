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
#include <glm/gtc/type_ptr.hpp>

const std::string c_shaderFilePath = "res/shaders/basic.shader";
const std::string c_textureFilePath1 = "res/textures/container.jpg";
const std::string c_textureFilePath2 = "res/textures/awesomeface.png";
constexpr unsigned int c_width = 800;
constexpr unsigned int c_height = 600;

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
        glfwCreateWindow(c_width, c_height, "Learn OpenGL", nullptr, nullptr);

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
             0.0f,  1.0f,  0.0f, 0.5f, 1.0f,
            -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 1, 3,
            0, 2, 4,
            0, 3, 4,
        };

        // Vertex Array Object
        VertexArray VAO;

        // Vertex Buffer Object
        VertexBuffer VBO(positions, sizeof(positions));

        // Element Buffer Object
        IndexBuffer EBO(indices, 12);

        VertexBufferLayout layout;
        layout.push<float>(3); // add the positions layout
        layout.push<float>(2); // add the texture layout
        // layout.push<float>(3) -> say we decide to add colors too
        VAO.addBuffer(VBO, layout); // this does -> VAO.bind() & VBO.bind()

        shader.bind();

        Texture texture1(c_textureFilePath1);
        shader.setUniform1i("u_texture1", 0);
        texture1.bind();

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(c_width/c_height), 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model1 = glm::mat4(1.0f);

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        float rotation = 0.0f;
        double prevTime = glfwGetTime();

        glCall(glEnable(GL_DEPTH_TEST));
        while (!glfwWindowShouldClose(window))
        {
            // Handle Input
            processInput(window);

            // Handle Render
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            double currTime = glfwGetTime();
            if (currTime - prevTime >= 1/30)
            {
                rotation += 0.5f;
                prevTime = currTime;

                if (rotation > 1.0f)
                {
                    rotation = 0.5f;
                }
            }

            model1 = glm::rotate(model1, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvp1 = proj * view * model1;

            shader.setUniformMat4f("u_MVP", mvp1);
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
