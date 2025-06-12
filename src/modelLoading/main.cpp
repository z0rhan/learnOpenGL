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
#include <stb_image/stb_image.h>
#include <iostream>
#include <format>
#include <filesystem>

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
glm::vec3 lightCubePos = glm::vec3(0.0f, 0.0f, 5.0f);

// Declarations
void updateWindowSize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void handleMouseMovement(GLFWwindow* window, double xPos, double yPos);
void handleMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
unsigned int loadTexture(const char* filePath);

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


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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

unsigned int loadTexture(const char* filePath)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    stbi_set_flip_vertically_on_load(true);
    int widht, height, nrComponents;
    unsigned char* data = stbi_load(filePath, &widht, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
        {
            format = GL_RED;
        } else if (nrComponents == 3)
        {
            format = GL_RGB;
        } else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, format, widht, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture from: " << filePath << "\n";
        stbi_image_free(data);
    }

    return texture;
}
