/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#include <iostream>
#include "defc.h"
#include "sound.h"

extern int Verbose;

extern int g_audio_rate;

void check_wave_error(int res, char *str);

extern int g_audio_enable;
extern word32 *g_sound_shm_addr;
extern int g_preferred_rate;

void generic_sound_init(word32 *shmaddr) {
    child_sound_loop(-1, -1, shmaddr);
}

void generic_sound_shutdown() {}

void check_wave_error(int res, char *str) {}

void child_sound_init() {
    set_audio_rate(g_audio_rate);
}

extern std::mutex *ex_mutex;
extern struct SoundOutputState *ex_soundState;

int generic_send_audio(byte *ptr, int in_size) {
    std::lock_guard<std::mutex> lock{*ex_mutex};

    std::vector<uint8_t> buffer;
    buffer.insert(buffer.begin(), ptr, ptr + in_size);

    auto event = SoundEvent {
      .buffer_ = buffer,
      .size_ = static_cast<unsigned int>(in_size)
    };

    ex_soundState->queue_.push_back(event);

    return in_size;
}
