## Enhel Headless KEGS emulator

Built on the [KEGS/SDL emulator](http://devernay.free.fr/apple2/) (before porting to SDL, at version 0.63) through rough removal of platform-dependent code to create compatibility with all possible compilers.

The code base has been rewritten for C++ compatibility, and portable entities have been added for easier use (mutexes, vectors, etc.)
It is easy enough to use with any framework that allows you to draw the graphical adapter data emulated device (originally planned to be used with the game framework Unity).

All rights reserved by the original author of the emulator (...Kent Dickey?).

## How to use

Simply add a directory using `add_subdirectory`, and add include files using `include_directories`. Then link the library using `target_link_libraries`.
The emulator will look for kegs.config (similar to KEGS) in application root and dependent files when started using StartEmulator.

## Interface description

```
// Run emulator (in a separate thread)
DllExport void StartEmulator();

// Get a copy of the graphical output 
DllExport GraphicsOutputStatePayload GetImageData();

// Emit mouse movement delta
DllExport void SetMouseDelta(int x, int y);

// Emit mouse click
DllExport void SetMouseClick(bool state);

// Emit keyboard key was pressed or released
DllExport void PushKeyboardEvent(int button_value, bool is_pressed);

// Get generated audio samples 
DllExport SoundOutputState* GetSoundState();

```

## Structures description


```
// A copy of one frame state of graphics adapter 
// R and B channels swapped and alpha channel is 0
struct GraphicsOutputStatePayload {
        std::vector<uint8_t> data_;         // Raw framebuffer bytes 
        int width_, height_;                // Width and height of framebuffer (640, 400 - usuall values)
        int depth_;                         // Color depth in bits (32 - usuall values)
        int ready_;                         // (!TODO: delete)
};

// Queue of audio events (48 kHz, 16 bits, 2 channels)
struct SoundOutputState {
    std::deque<SoundEvent> queue_;
};

|
|       // nested structure
|       //
\-->    struct SoundEvent {
            // raw samples data
            std::vector<uint8_t> buffer_;
            // size of data placd in 'buffer_'
            unsigned int size_;
        };

```