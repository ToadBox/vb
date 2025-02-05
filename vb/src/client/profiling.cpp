#include "client/profiling.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

vb::Profiler::Profiler() {
    frametime_us = 0.0f;
    fps = 0.0f;

    time_current = glfwGetTime();
    time_last_frame = time_current;
    fps_smoothing = 0.9f;

    frameCount = 0;
}

void vb::Profiler::beginFrame() {
    time_current = glfwGetTime();
    frameCount++;
    if (time_current - time_last_frame >= 0.5f) {
        float measured_fps = (frameCount / (time_current - time_last_frame));
        fps = (measured_fps * fps_smoothing) + (fps * (1.0 - fps_smoothing));
        frameCount = 0;
        time_last_frame = time_current;
    }
}

void vb::Profiler::endFrame() {
    if (time_current - time_last_frame >= 0.375f) {
        float measured_frametime = glfwGetTime() - time_current;
        // frametime_us = (measured_frametime * fps_smoothing)*1e6 + (frametime_us * (1.0 - fps_smoothing));
        frametime_us = measured_frametime * 1e6;
    }
}

float vb::Profiler::getFPS() {
    return fps;
}

float vb::Profiler::getFrametimeUS() {
    return frametime_us;
}