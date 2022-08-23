#include <vector>
#include "wrapper.hpp"

namespace HeKegs {
    DllExport void StartEmulator() {
        accessLock_.lock();
        graphics_ = new GraphicsOutputState{
                .image_ = nullptr,
                .width_ = -1,
                .height_ = -1,
                .depth_ = 0,
                .ready_ = false
        };
        mouseInputState_ = new MouseInputState{
                .mouse_delta_ = {0, 0},
                .clicked_lmb_ = false
        };
        keyboardInputState_ = new KeyboardInputState;
        soundOutputState_ = new SoundOutputState;

        accessLock_.unlock();

        std::thread([]() {
            run(&accessLock_, graphics_,
                keyboardInputState_, mouseInputState_,
                soundOutputState_);
        }).detach();
    }

    DllExport void PushKeyboardEvent(int button_value, bool is_pressed) {
        auto event = KeyboardEvent{
                .key_value_ = button_value,
                .pressed_ = is_pressed
        };
        keyboardInputState_->queue_.push(event);
    }

    DllExport void SetMouseDelta(int x, int y) {
        int newPosition[2] = {mouseInputState_->mouse_delta_[0] + x,
                              mouseInputState_->mouse_delta_[1] + y};

        newPosition[0] = std::clamp(newPosition[0], 0, 640);
        newPosition[1] = std::clamp(newPosition[1], 0, 400);

        mouseInputState_->mouse_delta_[0] = newPosition[0];
        mouseInputState_->mouse_delta_[1] = newPosition[1];
    }

    DllExport void SetMouseClick(bool state) {
        mouseInputState_->clicked_lmb_ = state;
    }

    DllExport GraphicsOutputStatePayload GetImageData() {
        std::lock_guard<std::mutex> lock{accessLock_};

        auto data = std::vector<uint8_t>(graphics_->size_);
        data.insert(data.begin(), graphics_->image_, graphics_->image_ + graphics_->size_);

        GraphicsOutputStatePayload payload = {
                .data_ = data,
                .width_ = graphics_->width_,
                .height_ = graphics_->height_,
                .depth_ = graphics_->depth_,
                .ready_ = graphics_->ready_
        };

        return payload;
    }

    DllExport SoundOutputState* GetSoundState() {
        std::lock_guard<std::mutex> lock{accessLock_};
        return soundOutputState_;
    }
}