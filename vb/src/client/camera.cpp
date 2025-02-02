#include "client/camera.hpp"

vb::Camera::Camera(glm::vec3 position, float yaw, float pitch) : position(position) {
    glm::quat yawRot = glm::angleAxis(glm::radians(yaw), up);
    glm::quat pitchRot = glm::angleAxis(glm::radians(pitch), glm::normalize(glm::cross(front, up)));
    orientation = glm::normalize(yawRot * pitchRot * orientation);
    front = orientation * glm::vec3(0.0, 0.0, -1.0);
}

void vb::Camera::Move(CameraMovementDirection direction, float dt) {
    float camera_speed = speed * dt;

    // utilizing provided direction, change the position of the camera
    switch (direction) {
        case CameraMovementDirection::FORWARD:
            position += camera_speed * front;
            break;
        case CameraMovementDirection::BACKWARD:
            position -= camera_speed * front;
            break;
        case CameraMovementDirection::LEFT:
            position -= camera_speed * glm::normalize(glm::cross(front, up));
            break;
        case CameraMovementDirection::RIGHT:
            position += camera_speed * glm::normalize(glm::cross(front, up));
            break;
        case CameraMovementDirection::UP:
            position += camera_speed * up;
            break;
        case CameraMovementDirection::DOWN:
            position -= camera_speed * up;
            break;
        default:
            break;
    }
    // regenerate look at orientation quaternion
    // orientation = glm::quatLookAtRH(glm::normalize(position + front), up);
}

void vb::Camera::Look(float xoff, float yoff) {
    float angleZ = invertX * xoff * sensitivity;
    float angleY = invertY * yoff * sensitivity;

    // construct quaternions to represent rotations about the left camera axis and the world up axis.
    glm::quat yawRot = glm::angleAxis(glm::radians(angleZ), -up);
    glm::quat pitchRot = glm::angleAxis(glm::radians(angleY), glm::normalize(glm::cross(front, up)));

    // update front quaternion by applying these delta rotations
    orientation = glm::normalize(yawRot * pitchRot * orientation);
    front = orientation * glm::vec3(0.0, 0.0, -1.0);
}

const glm::mat4 vb::Camera::View() const {
    return glm::lookAtRH(position, position+front, up);
}