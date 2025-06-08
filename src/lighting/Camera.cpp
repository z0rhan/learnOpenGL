#include "Camera.hh"

#include <iostream>
#include <cmath>

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 yAxis):
    m_yaw(c_yaw),
    m_pitch(c_pitch),
    m_cameraSensitivity(c_cmaeraSensitivity),
    m_cameraSpeed(c_cameraSpeed),
    m_fieldOfView(c_fieldOfView)
{
    m_cameraPos = cameraPos;
    m_cameraFront = cameraFront;
    m_yAxis = yAxis;
    updateCamera();
}

Camera::Camera(float posX, float posY, float posZ,
               float frontX, float frontY, float frontZ,
               float upX, float upY, float upZ)
{
    Camera(glm::vec3(posX, posY, posZ), glm::vec3(frontX, frontY, frontZ), glm::vec3(upX, upY, upZ));
}

//------------------------------------------------------------------------------
// GETTERs
float Camera::fieldOfView() const
{
    return m_fieldOfView;
}

glm::mat4 Camera::viewMat4f() const
{
    return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_yAxis);
}

glm::vec3 Camera::position() const
{
    return m_cameraPos;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Camera::processKeyboardInputs(MovementType movement, float& deltaTime)
{
    float movementSpeed = deltaTime * m_cameraSpeed;
    switch (movement)
    {
        case MovementType::FORWARD:
            m_cameraPos += m_cameraFront * movementSpeed;
            break;
        case MovementType::BACKWARD:
            m_cameraPos -= m_cameraFront * movementSpeed;
            break;
        case MovementType::RIGHT:
            m_cameraPos += m_cameraRight * movementSpeed;
            break;
        case MovementType::LEFT:
            m_cameraPos -= m_cameraRight * movementSpeed;
            break;
        default:
            std::cerr << "Wtf are you calling?\n";
            break;
    } 
}

void Camera::processMouseMovement(float xOffset, float yOffset, const bool constrainPitch)
{
    xOffset *= m_cameraSensitivity;
    yOffset *= m_cameraSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

    updateCamera();
}

void Camera::processMouseScroll(float yOffset)
{
    m_fieldOfView -= yOffset;

    if (m_fieldOfView < 1.0f) m_fieldOfView = 1.0f;
    if (m_fieldOfView > 45.0f) m_fieldOfView = 45.0f;

    updateCamera();
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// PRIVATE IMPLEMENTATIONS
void Camera::updateCamera()
{
    glm::vec3 cameraFront;
    cameraFront.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    cameraFront.y = sin(glm::radians(m_pitch));
    cameraFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_cameraFront = glm::normalize(cameraFront);
    m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_yAxis));
    m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}
