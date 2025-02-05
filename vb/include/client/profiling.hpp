#ifndef VB_PROFILING_HPP
#define VB_PROFILING_HPP

namespace vb {

class Profiler {
public:
    Profiler();

    void beginFrame();
    void endFrame();

    float getFPS();
    float getFrametimeUS();
private:
    float frametime_us;
    float fps;

    float time_current;
    float time_last_frame;
    float fps_smoothing;

    int frameCount;
};

}

#endif