cmake_minimum_required(VERSION 3.16)

project(vb)

# We only code with usable c++ in my house, thanks
set(CMAKE_CXX_STANDARD 20)
add_compile_options(
    -Wall -Wextra -pedantic -Wformat=2 -Wfloat-equal -fsanitize=address -fsanitize=undefined
)
add_link_options(
    -Wall -Wextra -pedantic -Wformat=2 -Wfloat-equal -fsanitize=address -fsanitize=undefined
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(vb vb/src/vb.cpp vb/src/core/game.cpp vb/src/core/shader.cpp vb/src/client/camera.cpp vb/src/client/input.cpp)

target_include_directories(vb PRIVATE ${CMAKE_SOURCE_DIR}/vb/include ${CMAKE_SOURCE_DIR}/external)

# Add GLFW
add_subdirectory(glfw)
target_link_libraries(vb glfw X11 pthread Xrandr Xi dl)
# fix GLFW not using good warnings, SHAME!!! SHAME!!!
target_compile_options(glfw PRIVATE -Wno-unused-parameter -Wno-pedantic -Wno-float-equal -Wno-sign-compare -Wno-missing-field-initializers)

# Add OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_link_libraries(vb OpenGL::GL)

# Add GLM
add_subdirectory(glm)
target_link_libraries(vb glm::glm)

# Add glad
add_subdirectory(glad/cmake)
glad_add_library(glad_gl_core_33 STATIC REPRODUCIBLE LOADER API gl:core=3.3)
target_link_libraries(vb glad_gl_core_33)

#imgui
target_include_directories(vb PRIVATE imgui/ imgui/backends)
target_sources(vb PRIVATE imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)

# Add spdlog
add_subdirectory(spdlog)
target_link_libraries(vb spdlog::spdlog)
target_compile_options(spdlog PRIVATE -Wno-unused-parameter -Wno-pedantic -Wno-float-equal)
