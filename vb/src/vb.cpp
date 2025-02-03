#include <cstdio>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/shader.hpp"
#include "client/camera.hpp"
#include "client/input.hpp"

namespace {
    float vertices[] = {
        // Vertices         UV Tex Coords
        // Face 1
        1.0, 1.0, -1.0,     1.0, 1.0,
        1.0, 1.0, 1.0,      1.0, 0.0,
        -1.0, 1.0, 1.0,     0.0, 0.0,
        -1.0, 1.0, -1.0,    0.0, 1.0,

        // Face 2
        1.0, 1.0, 1.0,      1.0, 1.0,
        1.0, -1.0, 1.0,     1.0, 0.0,
        -1.0, -1.0, 1.0,    0.0, 0.0,
        -1.0, 1.0, 1.0,     0.0, 1.0,

        // Face 3
        1.0, 1.0, -1.0,     1.0, 1.0,
        1.0, -1.0, -1.0,    1.0, 0.0,
        1.0, -1.0, 1.0,     0.0, 0.0,
        1.0, 1.0, 1.0,      0.0, 1.0,

        // Face 4
        -1.0, 1.0, 1.0,     1.0, 1.0,
        -1.0, -1.0, 1.0,    1.0, 0.0,
        -1.0, -1.0, -1.0,   0.0, 0.0,
        -1.0, 1.0, -1.0,    0.0, 1.0,

        // Face 5
        -1.0, 1.0, -1.0,    1.0, 1.0,
        -1.0, -1.0, -1.0,   1.0, 0.0,
        1.0, -1.0, -1.0,    0.0, 0.0,
        1.0, 1.0, -1.0,     0.0, 1.0,

        // Face 6
        1.0, -1.0, 1.0,     1.0, 1.0,
        1.0, -1.0, -1.0,    1.0, 0.0,
        -1.0, -1.0, -1.0,   0.0, 0.0,
        -1.0, -1.0, 1.0,    0.0, 1.0
    };
    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3,       // TOP (1)
        4, 5, 6, 4, 6, 7,       // SIDE 2
        8, 9, 10, 8, 10, 11,    // SIDE 3
        12, 13, 14, 12, 14, 15, // SIDE 4
        16, 17, 18, 16, 18, 19, // SIDE 5
        20, 21, 22, 20, 22, 23  // BOT (6)
    };

    unsigned int window_width = 0;
    unsigned int window_height = 0;

    float dt;
    float last_frame = 0.0f;
}

// callback for window resize
void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
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
    spdlog::set_level(spdlog::level::debug);

    // setup input and camera
    vb::Camera camera;
    vb::Input input;

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

    // set viewport and bind callbacks for window resizing and input
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    input.init(window, &camera);
    window_width = framebufferWidth;
    window_height = framebufferHeight;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);

    // generate VAO, VBO, and EBO for a single cube
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    vb::Shader block_shader("/home/yameat/Desktop/Programming/vb/vb/shaders/block/block.vs", "/home/yameat/Desktop/Programming/vb/vb/shaders/block/block.fs");

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    const char* loc = "/home/yameat/Desktop/Programming/vb/assets/blocks/missing.png";
    unsigned char *data = stbi_load(loc, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        spdlog::error("Failed to load texture {}", loc);
    }
    stbi_image_free(data);
    // Initialize ImGui
	  IMGUI_CHECKVERSION();
	  ImGui::CreateContext();
	  ImGuiIO& io = ImGui::GetIO(); (void)io;
	  ImGui::StyleColorsDark();
	  ImGui_ImplGlfw_InitForOpenGL(window, true);
	  ImGui_ImplOpenGL3_Init("#version 330");

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // update dt
        float current_frame = glfwGetTime();
        dt = current_frame - last_frame;
        last_frame = current_frame;
        // process input events
        input.processInput(dt);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        // New ImGui frame
		    ImGui_ImplOpenGL3_NewFrame();
		    ImGui_ImplGlfw_NewFrame();
		    ImGui::NewFrame();
        block_shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / window_height, 0.1f, 1000.0f);
        block_shader.setMat4("projection", &projection);
        glm::mat4 view = camera.View();
        block_shader.setMat4("view", &view);
        glm::vec3 model = glm::vec3(1.0f);
        block_shader.setVec3("model", &model);

        // spdlog::debug("Camera Pos: {}, {}, {}", camera.Position().x, camera.Position().y, camera.Position().z);
        // spdlog::debug("View Matrix: \n{}\n{}\n{}\n{}", glm::to_string(view[0]), glm::to_string(view[1]), glm::to_string(view[2]), glm::to_string(view[3]));
        // spdlog::debug("Projection Matrix: \n{}\n{}\n{}\n{}", glm::to_string(projection[0]), glm::to_string(projection[1]), glm::to_string(projection[2]), glm::to_string(projection[3]));
        
        // draw cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        if (input.getContext()->debug) {
            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Position: x: %0.6f, y: %0.6f, z: %0.6f", camera.Position().x, camera.Position().y, camera.Position().z);
            ImGui::End();
        }
        ImGui::Render();
		    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
	  ImGui_ImplGlfw_Shutdown();
  	ImGui::DestroyContext();

    glfwTerminate();
}