#include "bindings.hpp"

#include <cstdlib>
#include <cstdint>
#include <thread>
#include <mutex>
#include <vector>

#include "emulator/protos_generic_driver.h"

namespace HeKegs {
    struct GraphicsOutputStatePayload {
        std::vector<uint8_t> data_;
        int width_, height_;
        int depth_;
        int ready_;
    };

    static std::mutex accessLock_;

    static GraphicsOutputState *graphics_ = nullptr;
    static MouseInputState *mouseInputState_ = nullptr;
    static KeyboardInputState *keyboardInputState_ = nullptr;
    static SoundOutputState * soundOutputState_ = nullptr;

    DllExport void PushKeyboardEvent(int button_value, bool is_pressed);

    DllExport void StartEmulator();

    DllExport void SetMouseDelta(int x, int y);

    DllExport void SetMouseClick(bool state);

    DllExport GraphicsOutputStatePayload GetImageData();

    DllExport SoundOutputState* GetSoundState();
}