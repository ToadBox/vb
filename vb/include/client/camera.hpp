#ifndef VB_CAMERA_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>

namespace vb {

enum class CameraMovementDirection : uint8_t {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), float yaw = -90, float pitch = 0);

    const glm::mat4 View() const;

    void Move(CameraMovementDirection direction, float dt);
    void Look(float xoff, float yoff);

    glm::vec3 Position() const {return position;};

    void setSensitivity(float sensitivity);
private:
    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 proj_front;
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float pitch = 0;
    float yaw = 0;

    float speed = 2.5f;
    float sensitivity;
    float zoom;
    float invertX = 1.0f;
    float invertY = 1.0f;
};

}

#endif