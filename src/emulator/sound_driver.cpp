/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#include "defc.h"
#include "sound.h"

extern int Verbose;

extern int g_audio_rate;

int g_preferred_rate = 48000;
int g_audio_socket = -1;
int g_bytes_written = 0;

#define ZERO_BUF_SIZE        2048

word32 g_snd_zero_buf[ZERO_BUF_SIZE];

#define ZERO_PAUSE_SAFETY_SAMPS        (g_audio_rate >> 5)
#define ZERO_PAUSE_NUM_SAMPS        (4*g_audio_rate)

int g_zeroes_buffered = 0;
int g_zeroes_seen = 0;
int g_sound_paused = 0;
int g_childsnd_vbl = 0;
int g_childsnd_pos = 0;
word32 *g_childsnd_shm_addr = 0;

void reliable_buf_write(word32 *shm_addr, int pos, int size) {
    byte *ptr;
    int ret;

    if (size < 1 || pos < 0 || pos > SOUND_SHM_SAMP_SIZE ||
        size > SOUND_SHM_SAMP_SIZE ||
        (pos + size) > SOUND_SHM_SAMP_SIZE) {
        printf("reliable_buf_write: pos: %04x, size: %04x\n", pos, size);
        exit(1);
    }

    ptr = (byte *) &(shm_addr[pos]);
    size = size * 4;

    while (size > 0) {
        ret = generic_send_audio(ptr, size);

        if (ret < 0) {
            printf("audio write, errno: %d\n", errno);
            exit(1);
        }
        size = size - ret;
        ptr += ret;
        g_bytes_written += ret;
    }

}

void reliable_zero_write(int amt) {
    int len;

    while (amt > 0) {
        len = MIN(amt, ZERO_BUF_SIZE);
        reliable_buf_write(g_snd_zero_buf, 0, len);
        amt -= len;
    }
}


void child_sound_loop(int read_fd, int write_fd, word32 *shm_addr) {
    word32 tmp;
    int ret;

    doc_printf("Child pipe fd: %d\n", read_fd);

    g_audio_rate = g_preferred_rate;

    g_zeroes_buffered = 0;
    g_zeroes_seen = 0;
    g_sound_paused = 0;

    g_childsnd_pos = 0;
    g_childsnd_vbl = 0;
    g_childsnd_shm_addr = shm_addr;

    tmp = g_audio_rate;

    child_sound_init();

//    child_sound_playit(tmp);
}

void child_sound_playit(word32 tmp) {
    int size = tmp & 0xffffff;

    if ((tmp >> 24) == 0xa2) {
#if 0
        g_childsnd_pos += g_zeroes_buffered;
        while(g_childsnd_pos >= SOUND_SHM_SAMP_SIZE) {
            g_childsnd_pos -= SOUND_SHM_SAMP_SIZE;
        }
#endif

        if (g_zeroes_buffered) {
            reliable_zero_write(g_zeroes_buffered);
        }

        g_zeroes_buffered = 0;
        g_zeroes_seen = 0;

        if ((size + g_childsnd_pos) > SOUND_SHM_SAMP_SIZE) {
            reliable_buf_write(g_childsnd_shm_addr, g_childsnd_pos,
                               SOUND_SHM_SAMP_SIZE - g_childsnd_pos);
            size = (g_childsnd_pos + size) - SOUND_SHM_SAMP_SIZE;
            g_childsnd_pos = 0;
        }

        reliable_buf_write(g_childsnd_shm_addr, g_childsnd_pos, size);

        if (g_sound_paused) {
            printf("Unpausing sound, zb: %d\n", g_zeroes_buffered);
            g_sound_paused = 0;
        }

    } else if ((tmp >> 24) == 0xa1) {
        if (g_sound_paused) {
            if (g_zeroes_buffered < ZERO_PAUSE_SAFETY_SAMPS) {
                g_zeroes_buffered += size;
            }
        } else {
            /* not paused, send it through */
            g_zeroes_seen += size;

            reliable_zero_write(size);

            if (g_zeroes_seen >= ZERO_PAUSE_NUM_SAMPS) {
                printf("Pausing sound\n");
                g_sound_paused = 1;
            }
        }
    } else {
        printf("tmp received bad: %08x\n", tmp);
        exit(3);
    }

    g_childsnd_pos += size;
    while (g_childsnd_pos >= SOUND_SHM_SAMP_SIZE) {
        g_childsnd_pos -= SOUND_SHM_SAMP_SIZE;
    }

    g_childsnd_vbl++;
    if (g_childsnd_vbl >= 60) {
        g_childsnd_vbl = 0;
#if 0
        printf("sound bytes written: %06x\n", g_bytes_written);
        printf("Sample samples[0]: %08x %08x %08x %08x\n",
            g_childsnd_shm_addr[0], g_childsnd_shm_addr[1],
            g_childsnd_shm_addr[2], g_childsnd_shm_addr[3]);
        printf("Sample samples[100]: %08x %08x %08x %08x\n",
            g_childsnd_shm_addr[100], g_childsnd_shm_addr[101],
            g_childsnd_shm_addr[102], g_childsnd_shm_addr[103]);
#endif
        g_bytes_written = 0;
    }
}