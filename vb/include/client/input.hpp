#ifndef VB_INPUT_HPP
#define VB_INPUT_HPP

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "client/camera.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace vb {

enum class InputMode : uint8_t {
    PLAYER,
    RAW,
    SPECTATOR,
};

class Input {
public:
    struct shared_context {
        static bool debug;
    };

    static void init(GLFWwindow* window, Camera* camera);
    static void processInput(float dt);
    static int getKeyState(int key);
    static int getKeyMods(int key);
    static int getMouseButtonState(int button);
    static int getMouseButtonMods(int button);

    static void setInputMode(InputMode mode);
    static InputMode getInputMode();

    static shared_context* getContext();
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window,  double xoffset,  double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static std::unordered_map<int, int> key_state;
    static std::unordered_map<int, int> key_mods;
    static std::unordered_map<int, int> mouse_state;
    static std::unordered_map<int, int> mouse_mods;
    static InputMode currentMode;
    static Camera* camera;
    static GLFWwindow* window;
    static shared_context context;
};

}

#endif