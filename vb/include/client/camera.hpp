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
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), float yaw = 0, float pitch = 0);

    const glm::mat4 View() const;

    void Move(CameraMovementDirection direction, float dt);
    void Look(float xoff, float yoff);

    glm::vec3 Position() const {return position;};
    glm::quat Orientation() const {return orientation;};
private:
    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    /* currently unused, could be implemented to calculate camera up differently.
    * has extra implications on the rotation algorithm used for mouse input
    * camera front cross world_up = camera right
    * camera right cross camera front = camera up
    */
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::quat orientation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);

    float speed = 2.5f;
    float sensitivity = 0.01f;
    float zoom;
    float invertX = 1.0f;
    float invertY = 1.0f;
};

}

#endif