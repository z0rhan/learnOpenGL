#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float c_fieldOfView = 45.0f;
const float c_cameraSpeed = 2.5f;
const float c_cmaeraSensitivity = 0.1f;
const float c_yaw = -90.0f;
const float c_pitch = 0.0f;

enum class MovementType
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 yAxis);
    // For scalars
    Camera(float posX, float posY, float posZ,
           float frontX, float frontY, float frontZ,
           float upX, float upY, float upZ);

    void processKeyboardInputs(MovementType movement, float& deltaTime);
    void processMouseMovement(float xOffset, float yOffset, const bool constrainPitch);
    void processMouseScroll(float yOffset);

    // Getters
    float fieldOfView() const;
    glm::mat4 view() const;
    glm::vec3 position() const;
    glm::vec3 front() const;

private:
    glm::vec3 m_cameraPos;      // position
    glm::vec3 m_cameraFront;    // camera points to this direction
    glm::vec3 m_yAxis;          // y axis of the world
    glm::vec3 m_cameraRight;    // unit vector that points right of the camera
    glm::vec3 m_cameraUp;       // unit vector that points upwards of the camera


    float m_pitch;              // initial pitch angle
    float m_yaw;                // initial yaw angle
    float m_fieldOfView;        // as the name says
    float m_cameraSpeed;        // as teh name says
    float m_cameraSensitivity;  // as the name says

    // updates the above members on events
    void updateCamera();
};
