#include "client/input.hpp"

#include "spdlog/spdlog.h"

namespace {
    bool initialized = false;
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
    // temporary way to quickly exit the game
    if (getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (currentMode == vb::InputMode::SPECTATOR) {
        if (getKeyState(GLFW_KEY_W) == GLFW_PRESS) {
            camera->Move(vb::CameraMovementDirection::FORWARD, dt);
        }
        if (getKeyState(GLFW_KEY_S) == GLFW_PRESS) {
            camera->Move(vb::CameraMovementDirection::BACKWARD, dt);
        }
        if (getKeyState(GLFW_KEY_A) == GLFW_PRESS) {
            camera->Move(vb::CameraMovementDirection::LEFT, dt);
        }
        if (getKeyState(GLFW_KEY_D) == GLFW_PRESS) {
            camera->Move(vb::CameraMovementDirection::RIGHT, dt);
        }
        if (getKeyState(GLFW_KEY_C) == GLFW_PRESS) {
            camera->Move(vb::CameraMovementDirection::DOWN, dt);
        }
        if (getKeyState(GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera->Move(vb::CameraMovementDirection::UP, dt);
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

void vb::Input::keyCallback([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, int mods) {
    key_state[key] = action;
    key_mods[key] = mods;

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        context.debug = !context.debug;
    }
    spdlog::debug("Key pressed; key:{}, scancode:{}, action:{}, mods:{}", key, scancode, action, mods);
}

void vb::Input::mouseCallback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    static float lastX, lastY;
    static bool first_mouse = true;
	if (first_mouse)
	{
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

// todo: how tf
void vb::Input::scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]]  double xoffset, [[maybe_unused]]  double yoffset) {
    spdlog::debug("You scrolla da mouse wheel");
}

void vb::Input::mouseButtonCallback([[maybe_unused]] GLFWwindow* window, int button, int action, int mods) {
    mouse_state[button] = action;
    mouse_mods[button] = mods;
}