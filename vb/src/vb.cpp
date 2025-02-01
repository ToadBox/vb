#include <cstdio>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// callback for window resize
void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main([[maybe_unused]] int argc,[[maybe_unused]] char** argv) {
    // create console logger
    std::vector<spdlog::sink_ptr> console_sinks;
    console_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    console_sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/console.txt"));
    auto console = std::make_shared<spdlog::logger>("console", begin(console_sinks), end(console_sinks));
    spdlog::register_logger(console);
    spdlog::set_default_logger(console);
    spdlog::info("VB Startup");
    spdlog::info("SPDLog {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

    // setup window
    if (glfwInit() == GLFW_FALSE) {
        spdlog::critical("GLFW Window was unable to be initialized. Exiting.");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    GLFWwindow* window = glfwCreateWindow(800, 600, "VB", NULL, NULL);
    if (window == nullptr) {
        spdlog::critical("GLFW Window was unable to be created. Exiting.");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        spdlog::critical("Failed to initialize OpenGL context");
        glfwTerminate();
        return -1;
    }
    spdlog::info("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // set viewport and bind viewport callback for window resizing
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    

    glfwTerminate();
}