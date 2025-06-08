#include "error.hh"
#include "Shader.hh"
#include "Camera.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <format>
#include <filesystem>

const std::string c_cubeShader = "res/shaders/lighting/cube.shader";
const std::string c_lightCubeShader = "res/shaders/lighting/lightCube.shader";
constexpr unsigned int c_screenWidth = 800;
constexpr unsigned int c_screenHeight = 600;

// View related
float pitch = 0.0f;
float yaw = -90.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);

// initialize camera
Camera camera(cameraPos, cameraFront, yAxis);

// For time between each frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// For Mouse movement
float lastX = (float)c_screenWidth / 2;
float lastY = (float)c_screenHeight/ 2;
bool firstMouse = true;

// Light cube position
glm::vec3 lightCubePos = glm::vec3(0.0f, 0.0f, 4.0f);

// Declarations
void updateWindowSize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void handleMouseMovement(GLFWwindow* window, double xPos, double yPos);
void handleMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

int main (int argc, char *argv[])
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(c_screenWidth, c_screenHeight, "TEST", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to initialize window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, handleMouseMovement);
    glfwSetScrollCallback(window, handleMouseScroll);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!\n";
        return EXIT_FAILURE;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, updateWindowSize);

    glEnable(GL_DEPTH_TEST);

    std::cout << std::format("Wokring dir: {}\n", std::filesystem::current_path().string());

    float vertices[] =
    {
        // Positions          // Normal vectors
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Shader
    Shader cubeShader(c_cubeShader);
    if (!cubeShader.isValid())
    {
        std::cerr << "Error compiling shader\n";
    }

    Shader lightCubeShader(c_lightCubeShader);
    if (!lightCubeShader.isValid())
    {
        std::cerr << "Error compiling shader\n";

    }

    // Vertex Array Object for cube
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Vertex Buffer for cube
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex Array Object for cube
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // Vertex Buffer for cube
    // No need for new buffer since both are cube
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    // Still need to specify the layouts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // time between each frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rotate the light source
        glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat3 rotY = glm::mat3(rotMatrix);
        lightCubePos = rotY * lightCubePos;

        // Cube model
        glBindVertexArray(cubeVAO);
        cubeShader.bind();
        cubeShader.setUniformVec3("u_lightPosition", lightCubePos);
        cubeShader.setUniformVec3("u_objColor", glm::vec3(1.0f, 0.5f, 0.31f));
        cubeShader.setUniformVec3("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        cubeShader.setUniformVec3("u_viewPosition", camera.position());
        glm::mat4 model = glm::mat4(1.0f);
        cubeShader.setUniformMat4f("u_model", model);
        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.fieldOfView()), (float)c_screenWidth / (float)c_screenHeight, 0.1f, 100.0f);
        cubeShader.setUniformMat4f("u_projection", projection);
        // Camera
        glm::mat4 view = camera.viewMat4f();
        view = camera.viewMat4f();
        cubeShader.setUniformMat4f("u_view", view);

        // Draw call
        glCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        // lightCube model
        glBindVertexArray(lightCubeVAO);
        lightCubeShader.bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightCubePos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setUniformMat4f("u_model", model);
        lightCubeShader.setUniformMat4f("u_projection", projection);
        lightCubeShader.setUniformMat4f("u_view", view);

        // Draw call
        glCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glfwTerminate();

    return 0;
}

void updateWindowSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyboardInputs(MovementType::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyboardInputs(MovementType::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyboardInputs(MovementType::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyboardInputs(MovementType::RIGHT, deltaTime);
    }
}

void handleMouseMovement(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xOffset, yOffset, true);
}

void handleMouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(yOffset);
}

