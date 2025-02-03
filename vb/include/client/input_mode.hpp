#ifndef VB_INPUT_MODE_HPP
#define VB_INPUT_MODE_HPP

#include <cstdint>

namespace vb::input {
    enum class InputMode : uint8_t {
        RAW,      // No movement, raw keys only
        PLAYER,   // Player-type movement
        SPECTATOR // Spectator-type movement
    };

    namespace {
        InputMode current_mode = vb::input::InputMode::SPECTATOR;
    }

    InputMode inline getCurrentInputMode() {return current_mode;};
    void setCurrentInputMode(InputMode mode) {
        current_mode = mode;
        switch (current_mode) {
            case InputMode::RAW:
                
                break;
            case InputMode::SPECTATOR:

                break;
            
            case InputMode::PLAYER:

                break;
        }
    };
}

#endif