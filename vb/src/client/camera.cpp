#include "client/camera.hpp"
#include <spdlog/spdlog.h>

#define DEFAULT_SENSITIVITY 0.05f
#define MINIMUM_SENSITIVITY 0.001f
#define MAXIMUM_SENSITIVITY 1.0f

namespace {
    static const glm::vec3 X(1.0, 0.0, 0.0);
    static const glm::vec3 Y(0.0, 1.0, 0.0);
    static const glm::vec3 Z(0.0, 0.0, 1.0);
}

vb::Camera::Camera(glm::vec3 position, float yaw, float pitch) : position(position), pitch(pitch), yaw(yaw) {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    proj_front = glm::normalize(glm::dot(front, X)*X + glm::dot(front, Z)*Z);

    sensitivity = DEFAULT_SENSITIVITY;
}

void vb::Camera::Move(CameraMovementDirection direction, float dt) {
    float camera_speed = speed * dt;

    // utilizing provided direction, change the position of the camera
    switch (direction) {
        case CameraMovementDirection::FORWARD:
            position += camera_speed * proj_front;
            break;
        case CameraMovementDirection::BACKWARD:
            position -= camera_speed * proj_front;
            break;
        case CameraMovementDirection::LEFT:
            position -= camera_speed * glm::cross(front, world_up);
            break;
        case CameraMovementDirection::RIGHT:
            position += camera_speed * glm::cross(front, world_up);
            break;
        case CameraMovementDirection::UP:
            position += camera_speed * world_up;
            break;
        case CameraMovementDirection::DOWN:
            position -= camera_speed * world_up;
            break;
        default:
            break;
    }
}

void vb::Camera::Look(float xoff, float yoff) {
    yaw += invertX * xoff * sensitivity;
    pitch += invertY * yoff * sensitivity;

    // clamp pitch to avoid gimbal lock
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate the new front vector based on yaw and pitch
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Update the projected front vector
    proj_front = glm::normalize(glm::dot(front, X) * X + glm::dot(front, Z) * Z);
}

const glm::mat4 vb::Camera::View() const {
    return glm::lookAt(position, position+front, glm::vec3(0.0, 1.0, 0.0));
}

void vb::Camera::setSensitivity(float sens) {
    if (sens < MINIMUM_SENSITIVITY) sens = MINIMUM_SENSITIVITY;
    else if (sens > MAXIMUM_SENSITIVITY) sens = MAXIMUM_SENSITIVITY;
    
    sensitivity = sens;
}