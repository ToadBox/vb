#include "client/input.hpp"

#include "spdlog/spdlog.h"

#define KEY_DOWN (GLFW_PRESS | GLFW_REPEAT)

namespace {
    bool initialized = false;
    bool first_mouse = true;
}

std::unordered_map<int, int> vb::Input::key_state = {};
std::unordered_map<int, int> vb::Input::key_mods = {};
std::unordered_map<int, int> vb::Input::mouse_state = {};
std::unordered_map<int, int> vb::Input::mouse_mods = {};
vb::InputMode vb::Input::currentMode = InputMode::SPECTATOR;
vb::Camera* vb::Input::camera = nullptr;
GLFWwindow* vb::Input::window = nullptr;
vb::Input::shared_context vb::Input::context = {};
bool vb::Input::shared_context::debug = false;
bool vb::Input::shared_context::paused = false;

void vb::Input::init(GLFWwindow* _window, Camera* _camera) {
    if (!initialized) {
        assert(_window != nullptr);
        window = _window;

        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetScrollCallback(window, scrollCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        context.debug = false;
        assert(_camera != nullptr);
        camera = _camera;
    }
    initialized = true;
}

void vb::Input::processInput(float dt) {
    if (context.paused){
        /* code */
    } else {
        if (currentMode == vb::InputMode::SPECTATOR) {
            if (getKeyState(GLFW_KEY_W) & KEY_DOWN) {
                camera->Move(vb::CameraMovementDirection::FORWARD, dt);
            }
            if (getKeyState(GLFW_KEY_S) & KEY_DOWN) {
                camera->Move(vb::CameraMovementDirection::BACKWARD, dt);
            }
            if (getKeyState(GLFW_KEY_A) & KEY_DOWN) {
                camera->Move(vb::CameraMovementDirection::LEFT, dt);
            }
            if (getKeyState(GLFW_KEY_D) & KEY_DOWN) {
                camera->Move(vb::CameraMovementDirection::RIGHT, dt);
            }
            if (getKeyState(GLFW_KEY_C) & KEY_DOWN) {
                camera->Move(vb::CameraMovementDirection::DOWN, dt);
            }
            if (getKeyState(GLFW_KEY_SPACE) & KEY_DOWN) {
                camera->Move(vb::CameraMovementDirection::UP, dt);
            }
        }
    }
}

int vb::Input::getKeyState(int key) {
    return key_state[key];
}

int vb::Input::getKeyMods(int key) {
    return key_mods[key];
}

int vb::Input::getMouseButtonState(int button) {
    return mouse_state[button];
}

int vb::Input::getMouseButtonMods(int button) {
    return mouse_mods[button];
}

void vb::Input::setInputMode(InputMode mode) {
    currentMode = mode;
}

vb::InputMode vb::Input::getInputMode() {
    return currentMode;
}

vb::Input::shared_context* vb::Input::getContext() {
    return &context;
}

void vb::Input::keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, int mods) {
    key_state[key] = action;
    key_mods[key] = mods;

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        context.debug = !context.debug;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        context.paused = !context.paused;
        glfwSetInputMode(window, GLFW_CURSOR, context.paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        first_mouse = true;
    }
    
    // spdlog::debug("Key pressed; key:{}, scancode:{}, action:{}, mods:{}", key, scancode, action, mods);
}

void vb::Input::mouseCallback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    static float lastX, lastY;

    if (context.paused){
        /* code */
    } else {
        if (first_mouse) {
            lastX = xpos;
            lastY = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera->Look(xoffset, yoffset);
    }
}

// todo: how tf
void vb::Input::scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]]  double xoffset, [[maybe_unused]]  double yoffset) {
    spdlog::debug("You scrolla da mouse wheel");
}

void vb::Input::mouseButtonCallback([[maybe_unused]] GLFWwindow* window, int button, int action, int mods) {
    mouse_state[button] = action;
    mouse_mods[button] = mods;
}