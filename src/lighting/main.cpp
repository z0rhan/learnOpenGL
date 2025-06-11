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

const std::string c_cubeShader = "res/shaders/lighting/cube.shader";
const std::string c_lightCubeShader = "res/shaders/lighting/lightCube.shader";
const char* c_diffuseMap = "res/textures/lighting/container2.png";
const char* c_specularMap = "res/textures/lighting/test.png";
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

    float vertices[] =
    {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Shader
    Shader cubeShader(c_cubeShader);
    Shader lightCubeShader(c_lightCubeShader);

    // Vertex Array Object for cube
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    // Vertex Buffer for cube
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Vertex Array Object for cube
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // Vertex Buffer for cube
    // No need for new buffer since both are cube
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    // Still need to specify the layouts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture
    unsigned int diffuseMap = loadTexture(c_diffuseMap);
    unsigned int specularMap = loadTexture(c_specularMap);

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

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Rotate the light source
        glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat3 rotY = glm::mat3(rotMatrix);
        lightCubePos = rotY * lightCubePos;

        // Cube model
        glBindVertexArray(cubeVAO);
        cubeShader.bind();

        // Material
        // cubeShader.setUniformVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f)); // set the same as objColor
        // cubeShader.setUniformVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f)); // set the same as objColor
        cubeShader.setUniform1i("material.diffuse", 0);
        cubeShader.setUniform1i("material.specular", 1);
        cubeShader.setUniform1f("material.shininess", 32.0f);

        // Light
        cubeShader.setUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        cubeShader.setUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        cubeShader.setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        cubeShader.setUniformVec3("light.position", lightCubePos);
        cubeShader.setUniform1f("light.constant", 1.0f);
        cubeShader.setUniform1f("light.linear", 0.045f);
        cubeShader.setUniform1f("light.quadratic", 0.0075f);

        // torch like with spotlight
        cubeShader.setUniformVec3("torch.position", camera.position());
        cubeShader.setUniformVec3("torch.direction", camera.front());
        cubeShader.setUniform1f("torch.cutoff", glm::cos(glm::radians(10.5f)));
        cubeShader.setUniform1f("torch.outerCutoff", glm::cos(glm::radians(15.5f)));

        cubeShader.setUniformVec3("u_viewPosition", camera.position());

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setUniformMat4f("u_model", model);
            glCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        }

        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.fieldOfView()), (float)c_screenWidth / (float)c_screenHeight, 0.1f, 100.0f);
        cubeShader.setUniformMat4f("u_projection", projection);
        // Camera
        glm::mat4 view = camera.viewMat4f();
        view = camera.viewMat4f();
        cubeShader.setUniformMat4f("u_view", view);

        // lightCube model
        glm::mat4 model = glm::mat4(1.0f);
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
