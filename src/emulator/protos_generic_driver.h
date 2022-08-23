#pragma once

#include "defc.h"

struct GraphicsOutputState;
struct MouseInputState;

int run(std::mutex* globalLock,
        GraphicsOutputState *graphics,
        KeyboardInputState *keyboardInputState,
        MouseInputState* mouseInputState,
        SoundOutputState* soundOutputState);
void check_input_events(void);
void x_update_color(int col_num, int red, int green, int blue, word32 rgb);
void x_update_physical_colormap(void);
void show_xcolor_array(void);
void xdriver_end(void);
void x_get_kimage(Kimage *kimage_ptr);
void dev_video_init(void);
void x_redraw_status_lines(void);
void x_push_kimage(Kimage *kimage_ptr, int destx, int desty, int srcx, int srcy, int width, int height);
void x_push_done(void);
void x_auto_repeat_on(int must);
void x_auto_repeat_off(int must);
void x_hide_pointer(int do_hide);
